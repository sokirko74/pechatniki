"""
A script for cropping whitespace around images using ImageMagick.
It finds all images within a directory and its subdirectories and calls
ImageMagic's convert/trim command to crop them, storing the results in an output
directory (by default `cropped`).
Details
-------
You can specify options to only include files according to a pattern, exclude
files according to multiple patterns, and change the options for how ImageMagick
trims the images.
If you specify the *same* output directory as the input directory, then the
images are cropped in place.
Overwriting images can be avoided by specifying a prefix for the output images
(e.g., 'pictures/img.png' becomes 'pictures/cropped_img.png'), with the prefix
being automatically excluded to avoid creating a new cropped image after each
iteration of the script.
Usage:
    python trim_whitespace.py SRCDIR [-o OUTDIR, -p PREFIX, **additional_opts]
Examples:
    # Crop files in `./images` with default options
    python trim_whitespace.py ./images
    # Crop files in `./images` that have a `.png` extension, prefixing the
    # outputs with `crop_`, storing output in directory `out`
    python trim_whitespace.py ./images -i ".png" --prefix 'crop_' -o out
    # Crop files in `images`, storing the output in the same place but prefixed
    # with `crop_`
    # We automatically exclude files with the given prefix, to avoid creating
    # another file every time we crop the images.
    python trim_whitespace.py images -o images --prefix 'crop_'
Remarks
-------
I wanted to be able to add the cropped images in the same folder, but potentially
with a prefix (e.g., `crop_`) but was worried about doing it in a bash script
because of possible fragility leading to all my images being deleted.
Additionally, Python has the advantage of being able to identify images (with
the `imghdr` module), along with more convenient and apparently safer command
execution via `subprocess` and `shlex`.
"""

import argparse
import fnmatch
import glob
import imghdr
import os
import re
import shlex
import subprocess
import sys


def gen_find(topdir, pattern=None, excludes=list()):
    """Find all files contained within topdir and subdirectories, optionally
    specifying a pattern that the file names must satisfy, and additional
    patterns for file names to exclude.
    """
    if pattern is None:
        pattern = "*"
    if isinstance(excludes, str):
        excludes = [excludes]

    for root, dirlist, filelist in os.walk(topdir):
        for name in fnmatch.filter(filelist, pattern):
            for exclusion in excludes:
                if fnmatch.fnmatch(name, exclusion):
                    break
            else:
                yield os.path.join(root, name)


def filter_images(seq):
    """Filter a sequence of paths for ones associated with images."""
    for path in seq:
        if imghdr.what(path):
            yield path


def crop_image(path, dest, border="20x20", border_color="White"):
    """A wrapper around a shell command to ImageMagick for trimming whitespace."""
    cmd = [
        "convert",
        shlex.quote(path),
        "-trim",
        "-bordercolor",
        border_color,
        "-border",
        border,
        "+repage",
        shlex.quote(dest),
    ]
    print(" ".join(cmd))
    # return subprocess.check_output(cmd) # seems to throw errors unless shell?
    return subprocess.check_output(" ".join(cmd), shell=True)


def main(topdir, outdir, pattern=None, excludes=list(), prefix="", **kwargs):
    """The main whitespace-trimming logic."""
    # Get the files we wish to crop
    filepaths = filter_images(gen_find(topdir, pattern, excludes))

    # Get keyword arguments
    border = kwargs.get("border", "20x20")
    print ("aaaaa {}".format(border))
    border_color = kwargs.get("border_color", "White")
    dry_run = kwargs.get("dry_run", False)

    for path in filepaths:
        name = os.path.basename(path)
        # somewhat sketchy means of removing the top directory
        dirname = os.path.dirname(path).replace(topdir, "", 1).lstrip(os.sep)
        dirname = os.path.normpath(dirname)
        destname = prefix + name
        destdir = os.path.normpath(os.path.join(outdir, dirname))
        dest = os.path.join(destdir, destname)

        # Print the input file path and the destination for the cropped image
        print("%s \n\t--> %s" % (path, dest))

        # Create the directory if it doesn't exist and crop the image
        if not dry_run:
            os.makedirs(destdir, exist_ok=True)
            crop_image(path, dest, border=border)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "root", type=str, help="root directory to recursively search for images"
    )
    parser.add_argument(
        "--outdir",
        "-o",
        type=str,
        default="cropped",
        help="directory to store converted notebooks.",
    )
    parser.add_argument(
        "--include",
        "-i",
        type=str,
        help="File name pattern to filter the images by (e.g., `*.png`)",
    )
    parser.add_argument(
        "--exclude",
        action="append",
        default=list(),
        help="pattern for files to exclude.",
    )
    parser.add_argument(
        "--prefix",
        "-p",
        default="",
        help="Prefix to prepend to the cropped image's filename",
    )
    parser.add_argument(
        "--border",
        default="20x20",
        help="size of border around cropped image, in pixels",
    )
    parser.add_argument(
        "--border_color", default="White", help="Color of 'whitespace' to trim"
    )
    parser.add_argument(
        "--dry-run",
        default=False,
        action="store_true",
        help="Perform a dry-run, without cropping images changing files.",
    )
    parser.add_argument(
        "--exclude-prefix",
        default=True,
        action="store_false",
        help="A flag that tells the script to NOT exclude files with the"
        "supplied prefix. If you are cropping files in-place, this may lead to "
        "the creation of unwanted files after multiple iterations",
    )

    args = parser.parse_args()
    print(vars(args))

    exclusions = args.exclude
    if args.prefix:
        print("Adding prefix:", args.prefix + "*", "to exclusions...")
        exclusions.append(args.prefix + "*")

    main(
        args.root,
        args.outdir,
        pattern=args.include,
        excludes=exclusions,
        prefix=args.prefix,
        dry_run=args.dry_run,
        border=args.border,
        border_color=args.border_color,
    )
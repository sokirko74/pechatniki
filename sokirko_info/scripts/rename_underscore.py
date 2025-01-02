import os
import glob


def main():
    for root, _, _ in os.walk("."):
        if "_" in root:
            print(root)
            new_root = root.replace('_', '-')
            cmd = "mv {} {}".format(root, new_root)
            print(cmd)
            os.system(cmd)

    # for root, subdirs, files in os.walk("."):
    #     for f in files:
    #         pat h = os.path.join(root, f)
    #         if '_' in f:
    #             assert os.path.exists(path)
    #             nf = f.replace('_', '-')
    #             new_path = os.path.join(root, nf)
    #             cmd = "mv {} {}".format(path, new_path)
    #             print (cmd)
    #             os.system(cmd)

if __name__ == "__main__":
    main()

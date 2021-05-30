from bs4 import BeautifulSoup
import sys
import re
import os
import shutil

def normalize(text):
    if text is not None:
        text = " ".join(text.split())
        text = text.strip(' \n\t\r')
        return text
    return ""

def process_file(filename):
    with open (filename) as inp:
        soup = BeautifulSoup(inp, 'html.parser')
    body = soup.find("body")
    if body is None:
        return False
    table = soup.find("table")
    assert table is not  None
    row = table.find('tr')
    tds = list(row.find_all('td'))
    assert len(tds) > 0
    title = tds[0].encode_contents().decode('utf8').replace ("\n", " ")
    title = normalize(title).strip()
    assert (len(title) > 0)
    if len(title) > 150:
        print (title)
        assert (len(title) < 100)
    table_html = table.encode_contents().decode('utf8')

    youtube_match = re.search('http://youtu.be/([^"]+)"', table_html)
    assert youtube_match
    youtube_link = youtube_match.group(1)

    yandex_match = re.search('http://fotki.yandex.ru/users/([^"]+)"', table_html)
    assert yandex_match
    images_link = yandex_match.group(1)
    url_path = os.path.dirname(filename).replace('\\', '/')
    index = url_path.find('/Part')
    assert index != -1
    url_path = url_path[index:]
    title = title.replace("'", '"')
    print("'{}': ['{}', '{}', '{}'],".format(url_path, title, youtube_link, images_link))
    table.decompose()
    first_link = soup.find('a')
    assert first_link.encode_contents().decode('utf8').find('header2') != -1
    first_link.decompose()

    cnt = 0
    for l in soup.find_all('a'):
        href = l.attrs.get('href')
        if href is not None and href.startswith('http://creativecommons.org/licenses/by/4.0'):
            l.decompose()
            cnt += 1
    assert cnt == 2

    script_include = """
    <script type="text/javascript" src="/modules/utilities.js"></script>
    <script>
        document.addEventListener("DOMContentLoaded", function () {
            show_film_table_by_url_path(window.location.pathname)
        });
    </script>
    """
    shutil.copy(filename, filename+ ".sav")
    with open (filename, "w") as out:
        html = str(soup)
        html = re.sub('Все материалы сайта sokirko.info доступны по\s\.', '', html)
        html = re.sub('<body>\s*\n', '<body>\n' + script_include, html)
        out.write(html)
    return True


if __name__ == "__main__":
    if not process_file(sys.argv[1]):
        sys.exit(1)
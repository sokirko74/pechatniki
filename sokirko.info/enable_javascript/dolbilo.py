import sys
from selenium import webdriver

if __name__ == "__main__":
    selenium = webdriver.Firefox()
    with open(sys.argv[1]) as inp:
        for url in inp:
            print(url)
            url = url.strip()
            selenium.get(url)
            html = selenium.page_source
            assert html.find('youtu.be') != -1

    selenium.quit()

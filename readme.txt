1. В гитхабе https://github.com/sokirko74/pechatniki находится весь html, cs, javascript.
Проект запускается с помощью Django.

1.1

2. В проде работает nginx (конфиг в том же гитхабе) и  gunicorn (конфиг в том же гитхабе).
 sudo systemctl enable gunicorn_sokirko_info.socket
 sudo systemctl start gunicorn_sokirko_info.socket
 sudo systemctl enable gunicorn_sokirko_info.service
 sudo systemctl start gunicorn_sokirko_info.service
 sudo systemctl status gunicorn_sokirko_info.service

#test gunicorn
    /home/sokirko/pechatniki/sokirko_info$ gunicorn --env DJANGO_SETTINGS_MODULE=sokirko_info.settings --bind localhost:8000 sokirko_info.wsgi


3. HTTPs  настроен через letsencrypt, раз в месяц запускается обновление сертификата.
    sudo apt-get install certbot python3-certbot-nginx
    sudo certbot --nginx

4. Основные картинки вебсайта находятся на Яндекс диске https://disk.yandex.ru/d/GRNFs3NvJ-a0NA .
  Эти картинки должны возникнуть по пути ~/pechatniki/sokirko_info/sokirko_info/static
5.  Слайды в формате  jpg находятся на Яндекс-диске https://disk.yandex.ru/d/lxSdJ3OvBLy6oQ .
  Эти слайды должны возникнуть по пути ~/pechatniki/sokirko_info/sokirko_info/static/SlideFilms


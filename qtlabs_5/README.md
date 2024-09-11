# Описание

Тут буду публиковать задачи и свои решения к ним по курсу Qt&QML в МИФИ.

Скорее всего программировать буду на PyQt. На курсе ORM будет у нас peewee, пока не знаю почему именно она.

# Полезные материалы

- https://github.com/Erriez/pyside6-getting-started

- очевидно будет ещё что-то

# Установка

Так как делать всё будем на python, то и установка соответствующая:

(Так как я работаю на Archlinux, то и установка с запуском соответсвтующие)

```bash
python -m venv venv

source venv/bin/activate

git clone https://code.qt.io/pyside/pyside-setup

cd pyside-setup

pip install -r requirements.txt

python setup.py build --qtpaths=/opt/Qt/6.5.0/gcc_64/bin/qtpaths --build-tests --ignore-git --parallel=8
```

# Запуск

```bash
source venv/bin/activate

cd anywhere/you/want

python your_project.py

deactivate
```

# Вторая лаба

## Условие

Алгоритм рассеивания ошибки.

Реализовать преобразование изображения 8 bpp в n bpp (n < 8) с
использованием алгоритма рассеяния ошибки Флойда-Стенберга. Допускается
упрощенная реализация, при которой результат хранится в виде 8 bpp,
но каждый пиксель может иметь только одно из n-х допустимых значений.

## Доп задачи

1. В алгоритме рассеивания выполнять проход в противоположном
направлении для чётных и нечётных строк.
2. Реализовать алгоритм Stuki.

## Команда для запуска

```bash
./main --f=/home/vpert/GitRepos/labs/5_cg/pics/cube.png
```

## Полезные ссылки

- [Обзорная статья на хабре](https://habr.com/ru/articles/326936/)

- [Ordered dithering](https://en.wikipedia.org/wiki/Ordered_dithering)

- [Floyd–Steinberg dithering](https://en.wikipedia.org/wiki/Floyd%E2%80%93Steinberg_dithering)

- [Stucki Dithering](https://graphicsacademy.com/what_ditherstucki.php)

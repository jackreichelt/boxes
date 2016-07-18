# Boxes

## Dependencies

None

## Compilation

```sh
gcc -ansi -pedantic -o boxes boxes.c
```

## Playing the game

```sh
boxes height width playercount [filename]
```

Players then make moves by entering commands in the following format:
```sh
x y (h/v)
```
where x and y are coordinates, and h/v determines if the line is going horizontally or vertically.

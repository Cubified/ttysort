# ttysort

A simple, terminal-based sorting algorithm visualizer

## Demo

![demo](https://github.com/Cubified/ttysort/blob/master/demo.gif)

## Compilation

     git clone "https://github.com/Cubified/ttysort"
     cd ttysort
     make

And optionally (installs to `~/.local/bin`):

     make install

Note:  `ttysort` requires [`termbox`](https://github.com/nsf/termbox)

## Adding New Algorithms

All sorting algorithm functions should be in the format:

     void myalgo(elem *arr, int n, func disp)

Where the values of array members are accessed as:

     arr[0].val
     arr[1].val
     ...
     arr[n].val

Additionally, the `disp()` function should be called at some relatively consistent interval and be given the following arguments:

     disp(arr, n, [marker_point_1], [marker_point_2], [y_offset]);

(The latter three are optional, as simply passing `disp(arr, n, -1, -1, 0)` negates them)
The `disp()` function should be followed immediately by:

     usleep(RUN_SPEED);

To ensure the visualization runs at a reasonable speed.

Finally, algorithms should be added to the `options[]` array, and their associated names to the `option_names_pretty[]` array as well.

## To-Do

- Add additional sorting algorithms (merge, heap, tim, etc.)
- Add user-adjustable dataset size
- Fix broken backspace in input fields (left arrow works in its place)
- Fix cocktail shaker sort breaking occasionally
- Clean up codebase

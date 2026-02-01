# Window

## Initialize SimpleGUI

```c
void sg_init(SgSize size, const char *title);
```

This function initializes SDL2, creates a window
with the specified size and title.

## Close Window

```c
void sg_destroy(void):
```

Call this function at the end of your application
for cleaning up.

## Change window title

```c
void sg_set_title(const char *title);
```

## Begin frame

```c
void sg_begin(SgColor color);
```
This function will fill the entire window with the specified color.
Call at the beginning of a frame before drawing anything else.

## End frame

```c
void sg_update(void);
```

Call this function at the end of a frame when you are done
with rendering.

## Check for quit

```c
bool sg_running(void);
```

You can use this function as the condition for your
main loop, to check for when the window should be closed.

## Get window size

```c
SgSize sg_get_window_size(void);
```

This function returns the current window size.

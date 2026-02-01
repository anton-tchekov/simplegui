# Mouse and Keyboard input

## Mouse

### Mouse Button Definitions
```c
enum
{
	SG_BUTTON_LEFT = 1,
	SG_BUTTON_MIDDLE = 2,
	SG_BUTTON_RIGHT = 4
};
```

### Is Mouse Button down?

```c
int sg_is_mouse_button_down(int button);
```

Returns true if specified mouse button is currently down.

### Was Mouse Button pressed?

```c
int sg_is_mouse_button_pressed(int button);
```

Returns true if specified mouse button was pressed this frame.

### Was Mouse Button released?

```c
int sg_is_mouse_button_released(int button);
```

Returns true if specified mouse button was released this frame.

### Get Mouse Position

```c
SgPoint sg_mouse_position(void);
```

Returns current mouse position within window in pixels.

### Get Mouse Wheel Movement

```c
SgPoint sg_mouse_wheel(void);
```

Returns relative mouse wheel position.

## Keyboard

The key parameter is an `SDL_Scancode`.
[SDL_Scancode](https://wiki.libsdl.org/SDL2/SDL_Scancode)

### Is key down?

```c
int sg_is_key_down(int key);
```

Returns true if specified key is currently down.

### Was key pressed?

```c
int sg_is_key_pressed(int key);
```

Returns true if specified key was pressed this frame.

### Was key released?

```c
int sg_is_key_released(int key);
```

Returns true if specified key was released this frame.

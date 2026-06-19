# RustAPI TestMod

A comprehensive test mod for RustAPI that demonstrates all available API features and can be used to verify the API is working correctly.

## Features

This testmod demonstrates:
- **Keyboard Input Handling**: Listen for key presses and respond to them
- **File I/O Operations**: Read, write, and manipulate files
- **Web Requests**: Fetch data from the internet
- **Mod Settings**: Save and load mod configuration
- **UI Creation**: Create popups and interface elements
- **Node Manipulation**: Modify Cocos2d nodes in the game

## Installation

1. Build RustAPI first
2. Build this testmod: `cmake --build build --target RustAPITestMod`
3. Copy the resulting `.geode` file to your Geode mods folder
4. Enable both RustAPI and RustAPI TestMod in Geode

## Usage

Once loaded, the testmod will:
- Add a test button to the main menu
- Log initialization messages to the console
- Respond to keyboard input (K key shows a popup)
- Demonstrate various API features

## Testing Features

### Keyboard Test
Press the **K key** in-game to trigger a test popup that demonstrates keyboard input handling.

### File I/O Test
The mod will attempt to read/write files in its save directory to verify file operations work.

### Web Request Test
The mod can make test web requests to verify network functionality.

### Settings Test
The mod saves and loads settings to verify the configuration system works.

## Building

```bash
# Configure
cmake -B build

# Build
cmake --build build

# The .geode file will be in build/
```

## Development

This testmod serves as both:
1. A verification tool for RustAPI functionality
2. An example/reference for developers wanting to use RustAPI

Look at the source code to see how each API feature is used.

## Troubleshooting

If the testmod doesn't work:
1. Ensure RustAPI is loaded first (check dependencies in mod.json)
2. Check the Geode console for error messages
3. Verify both mods are enabled in Geode settings
4. Make sure you're using compatible versions

## License

Same as RustAPI

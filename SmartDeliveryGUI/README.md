# Smart Delivery Management System — GUI Edition

The exact same Smart Delivery system (packages, drivers, warehouse,
routes, dispatch, tracking, requests, reports, undo) — same data
structures, same core logic — with a real desktop GUI (Dear ImGui +
SFML) instead of the terminal TUI. This is a genuine window with mouse
clicks, buttons, forms, tables, and a couple of live-drawn visuals
(the road network map, and a hash-bucket occupancy chart).

**This has been compiled and actually run (with a screenshot verified)
before being handed to you** — it is not just code that looks right.

## What's the same, what's different

- `include/Global.h`, and the core logic in `Package.h/.cpp`,
  `Driver.h/.cpp`, `Graph.h/.cpp`, `BST.h/.cpp`, `MinHeap.h/.cpp`,
  `HashTable.h/.cpp`, `UndoStack.h/.cpp`, `RequestQueue.h/.cpp`,
  `SortSearch.h/.cpp` are the SAME data structures and functions as the
  console version — `addPackage`, `bstInsert`, `heapPush`, `shortestPath`,
  etc. all work identically underneath.
- What's gone: every `run...Screen()` function and all of `Console.h`
  (the `_getch()`/ANSI TUI engine) — none of that applies to a real
  window.
- What's new: `src/*Panel.cpp` — one file per module, each an ImGui panel
  (forms, tables, buttons) that calls the exact same core functions the
  console screens used to call. `src/main.cpp` opens an SFML window,
  initializes ImGui, and switches between panels via a sidebar instead of
  `runMenu()`.
- A few small additions were needed because the GUI lives in different
  files than the data it displays (the console screens could read a
  module's private array directly since they were in the same .cpp file;
  GUI panels can't) — see `BST.h`'s `bstInOrderCollect`, `MinHeap.h`'s
  `heapSnapshot`, `RequestQueue.h`'s `queuePeekAt`, and `Graph.h`'s
  `getAllRoads`. These are read-only accessors, not new data structures.

## One-time setup: install SFML (you already have MinGW from the console build)

Open the **MSYS2 UCRT64** terminal (the same one you used to install g++) and run:
```
pacman -S mingw-w64-ucrt-x86_64-sfml
```
That's the only new thing you need — Dear ImGui itself is already vendored
inside this project (`vendor/imgui`, `vendor/imgui-sfml`), so there's
nothing else to download.

## Build & run

In VS Code, open this `SmartDeliveryGUI` folder (not a parent folder —
same folder-nesting issue as last time, so double check with `dir` that
you see `src`, `include`, `vendor` before building).

Press **Ctrl+Shift+B**, or run manually:
```
g++ -std=c++17 -Iinclude -Ivendor/imgui -Ivendor/imgui-sfml src/Global.cpp src/Package.cpp src/Driver.cpp src/Graph.cpp src/BST.cpp src/MinHeap.cpp src/HashTable.cpp src/UndoStack.cpp src/RequestQueue.cpp src/SortSearch.cpp src/PackagePanel.cpp src/DriverPanel.cpp src/WarehousePanel.cpp src/GraphPanel.cpp src/DispatchPanel.cpp src/RequestPanel.cpp src/TrackingPanel.cpp src/ReportsPanel.cpp src/UndoPanel.cpp src/SettingsPanel.cpp vendor/imgui/imgui.cpp vendor/imgui/imgui_draw.cpp vendor/imgui/imgui_widgets.cpp vendor/imgui/imgui_tables.cpp vendor/imgui-sfml/imgui-SFML.cpp src/main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -o SmartDeliveryGUI.exe
```
Then:
```
.\SmartDeliveryGUI.exe
```

A real window should open (1280x800) with a sidebar on the left listing
every module, and the Package Management panel showing by default.

## Using it

- **Click** sidebar items to switch modules — no more arrow keys/Esc.
- Forms are normal text/number fields — click into a box, type, click the
  button.
- Tables (Packages, Drivers, Inventory, etc.) update live — add something
  and it appears immediately.
- **Delivery Routes** now has an actual visual map: locations are drawn as
  circles, roads as lines with distance labels, and after "Find Shortest
  Path" the route is redrawn in green over the map.
- **Tracking System** shows a bar per hash bucket — taller/orange bars
  mean a longer collision chain.
- **Driver history** uses "< Older" / "Newer >" buttons instead of arrow
  keys, walking the same doubly linked list.

## If the build fails

- `fatal error: SFML/Graphics.hpp: No such file or directory` → you
  haven't run the `pacman -S mingw-w64-ucrt-x86_64-sfml` step above.
- `cannot find -lsfml-graphics` → same fix; also make sure you're using
  the **UCRT64** MSYS2 environment (same one as your console build), not
  MINGW32/MINGW64.
- Any `src/....cpp: No such file or directory` → you're one folder level
  too high, same as with the console project — `cd` into the folder that
  directly contains `src`.

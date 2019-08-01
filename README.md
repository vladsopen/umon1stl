
# Umon

Umon is a platform-independent, framework-independent PoC application, which provides near real-time access to a remote monitoring device.

Umon includes its own simple C++-lambda-based GUI framework to segregate application code from unwanted external API dependencies. The current open source version compiles either to JavaScript using Emscripten or as a QT-based app for the desktop platforms.

# Online Demo

http://o-pn.com/umon/1b

# The Code

* Umon1 is STL based. Unfortunately, this exposes the application code to all historical STL design flaws and unsafe undefined behavior. This is to be remedied in the future with a safe template library API layer.

* Reliability features:

  * No pointer (raw or smart) is ever **NULL** unless explicitly declared as non-dereferenceable `weak_ptr` or named like `fooOrNull`, `getBarOrNull()`. Those aren't in use in the application code, but we have some in tests and system libs.

  * No divisions allowed outside of `div0(x, y, or z)` expression for any types, regardless of other zero checks.

  * Highest warning level, all warnings == errors. (Exception: shadowing is allowed because of nested `ui`. A linter will be used to sort out any unexpected shadows.)

  * All unfortunate STL `unsigned` sizes go through `int useful::size()` / `int useful::length()`.

  * `As<type, class StrictChecker>` template is used for potentially ambiguous public function parameters. `Maybe<>` is used for optional return values.

  * `Out` prefix for output parameters: `int foo(Out, int& result);` and `x = foo(Out(), &r);`

  * No global state. Even system and test code passes `Io` object around to encapsulate any system, logging and debugging states. Multiple independent applications should be able to run in the same process, using different frameworks, batch/GUI or debugging modes.

  * The code should be exception-safe. In the sense that exceptions are fatal, they always stop the application, and should never normally happen. Automatic process restart is being considered for improving Umon fault tolerance. (Even though we don't catch exceptions, no unsafe temporary states should exist in objects. See `Cleaner` template. Most rules for exception safety are still useful without throw/catches.)

  * The code is thread-safe. Assuming, of course, that all our parallel tasks communicate through well defined channels instead of drowning in an insane mess of shared global state, passing around OOP pointers, poisoned with mutexes and potential deadlocks. All application objects can assume that data encapsulation is not violated unexpectedly.

  * `rASSERT`/`rFAIL` are presumed to just silently log errors in the release build. But application should not assume this. They can be removed for unsafe high performance mode. They can alternatively crash in fail-fast mode. The code must still be able to recover from failing assertions for graceful degradation mode without causing undefined behavior, infinite loops or data corruption.

# Test Framework

*TBD*

(The tests extensively rely on automatic test log matching. There is a bank of verified test logs in `resources` directory. A GUI diff tool is automatically launched for any mismatch in test results)

# TODO

* Non-portable stuff implemented in the original [redacted] application currently removed from Umon1:

  * Incremental repaint: Currently Umon1 redraws polylines on every `onPaint` which is barely acceptable in JS build. We want a portable cache implementation which appends existing graphics to HTML canvas and maps to something similar at QT side.

  * Server communication: Umon1 only implements `Emulator` as the data source `Feeder`.

  * Local storage and offline scrolling: Umon1 just discards all data beyond the current view width.

  * Event list implementation and user defined markers.

* More todos:

  * Unit Tests should be able to match to logs in Emscripten build too.

  * Support for skins and nicer icons in `uiframework`.

  * The UI framework is really barebones. This is by design. Unlike the universal frameworks it will serve the priorities of Umon application, exposing the application only to the APIs it needs. But we still have to implement or provide proxies to QT and HTML for:
    * more flexible repaint 
    * widget tree and layout update
    * more standard controls
    * tooltips, pads/intervals, skins, icons, rich text, pop ups, dialog boxes etc.

  * Android native app mode through WebView and QT. (We do already support mobile browsers.)

  * *TBD*

# Application Structure

```
 MakeAppMainWindow
   |
   |--> MakeXxxx... toolbars, status, etc.
   |
   '--> MakeGraphics
         |      |
         |      v
         |     View ---- Scaler
         |      ^    |
     (onTimer)  |    '-- DrawXxxx
         |      |         |
         v      |         |
       Incrementer        |
         |                |
   ------+----------------v-----
  |      |             Storage  |
  |      v                      |
  |    Waves                    |
  |      |                      |
  |      v                      |
  |    Wave[]                   |
  |      |                      |
   ------+----------------------
         |
         v
      Feeder  <---- Emulator
                |-- Server
                |-- Device
```
    
*TBD*

# Properties 

A `GetSet<int> foo;`-style member is accessed just like a classical `get`/`set` pair:
```
    int n = object->foo();
    object->foo(13);
```
so it could easily be replaced with the proper `get`/`set` methods when needed and without any changes to the client code. See also the specialized `AssignOnly<>`, `SetScalar<>`, `const GetUnique<>`, `const GetRelated<>`, `const GetParent<>`, `const GetPredefined<>` templates. And `GetSetTell<>` variations with subscriber/broadcast support for UI updates. More *TBD*.

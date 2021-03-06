# IconvQtPOC: Integrating iconv() for Unicode conversion with a Qt app (POC)

This project is a proof-of-concept for using
[iconv(3)](https://www.unix.com/man-page/osf1/3/iconv/)
to convert text from any charset to [Unicode](https://home.unicode.org/),
integrated in a Qt app.

## Why this exists?

Qt has a convenient class:
[QTextCodec](https://doc.qt.io/qt-5/qtextcodec.html),
that performs conversion between Unicode and other character sets, until Qt5.
In Qt6 this class was moved from QtCore to the
[Qt5Compat module](https://doc.qt.io/qt-6/qtcore5compat-module.html),
which means that it will be relegated in maintenance
priority, and it may be totally removed in the future.

Drumstick::File is a library, belonging to the
[Drumstick project](https://drumstick.sourceforge.io/), that supports
Input/Output from
[Standard MIDI Files](https://www.midi.org/specifications-old/item/standard-midi-files-smf)
(.MID), which include text metadata, like
lyrics, comments, credits, copyrights, instruments, marks and cue names. Some
programs based on Drumstick (like
[kmidimon](https://kmidimon.sourceforge.io/) and
[dmidiplayer](https://dmidiplayer.sourceforge.io/)
) display metadata to the users, so the text needs to be converted to Unicode.

## SMF metadata

[The SMF standard](https://www.midi.org/specifications-old/item/standard-midi-files-smf)
was created as a file interchange format, so users could export
compositions from one MIDI sequencer proprietary format, and import those files
into another sequencer. Some sequencer hardware machines and software became
deprecated and unusable with time, and the data files in proprietary format
became unreadable, losing work unless the users kept SMF exported files. Those
files should now be treated as original manuscripts for data preservation.

The authors of the standard, naively stated that all text metadata should be
encoded in ASCII within a MIDI file, omitting the fact that many cultures exist
around the world needing other encodings to reliable store their languages. Of
course, the result was that composers, manufacturers and software developers
all over the world ignored that standard requirement and used whatever encoding
that was available at the time. Nowadays, a good encoding choice is UTF-8, and
this is what [Rosegarden](https://www.rosegardenmusic.com/)
does when exports compositions as MIDI files. But for
importing from MIDI files, software apps need to convert from some unknown
encodings into Unicode for processing.

## Tested solution

This project does not read SMF files. Instead, it reads plain text files with
unknown encodings, and tries to detect the input charset using
[uchardet](https://www.freedesktop.org/wiki/Software/uchardet/). The user has a
control to choose another charset using a combobox, if the detected one is not
correct. Then, the input data is converted to Unicode by **iconv(3)** and
displayed to the user.

There is another POC, functionally equivalent,
[alternative using ICU](https://github.com/pedrolcl/icu_qt_poc).

CMake minimum requirement is v3.11, because
[FindIconv.cmake](https://cmake.org/cmake/help/v3.11/module/FindIconv.html)
was introduced in that version.

iconv() is a POSIX API, and it is included in the C library of many Unix
systems, like the GNU libc. This may be detected by cmake, and in this case no
additional library needs to be linked to the application program, but the
external [libiconv](https://www.gnu.org/software/libiconv/) is also supported
when needed.

Character set detection was never provided by Qt. This POC uses the latest
uchardet library (0.0.7 at the time of writting this), which only supports
pkg-config. A newer version will include also support for find_package()
and imported targets.

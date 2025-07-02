# days2live

A C++ program that shows, in real time, how many years, months, days, hours, minutes, and seconds remain in the average human lifetime, given a birth date. The program leverages C++20/23 standard library features for date, time, and timezone handling, and displays the result with high precision.

## Features

- Displays time left to live, or time since death, with microsecond precision.
- Uses your system's time zone (or can be adjusted in the code).
- Modern C++ (20/23) only; no third-party date libraries required.
- Fully terminal-based output with live updates.
- Lifetime (`years_to_life`) and birthday (`bday`) can be set as command line arguments.
- Built-in help option: `-h` or `--help`.

## Build

Requires:
- GCC 13+ (or Clang with libc++ 17+)
- C++20 standard library with time zone support (`std::chrono::zoned_time`)

Example build command:
```
g++ -std=gnu++20 days2live.cpp -o days2live
```

## Usage

```
./days2live [years_to_life] [YYYY-MM-DD]
```

- `years_to_life` (optional): The number of years considered as the average human lifetime. Default is 125 if not specified.
- `YYYY-MM-DD` (optional): Your birthday in format YYYY-MM-DD. Default is 1978-10-09 if not specified.
- `-h`, `--help`: Show help message and exit.

### Examples

```sh
./days2live 90 1982-10-26
```

By default, the program is set to October 9, 1978 as the birthdate and 125 years as the average lifetime.

## License

MIT License (see [LICENSE](LICENSE))

## Author

- [koppi](https://github.com/koppi)
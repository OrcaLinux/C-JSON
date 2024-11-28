# Changelog

## [1.1.0] - 2024-10-29

### Added

-   **Logging and Debugging Features**:

    -   Added conditional `DEBUG_PRINT` macro for enabling debug prints at compile-time.
    -   Added `ERROR_LOG` macro to conditionally enable error logging via `fprintf`, controlled by the `ERROR_LOGGING_ENABLED` flag in `json_config.h`.

-   **New Header Files**:
    -   `json_config.h`: Config file for enabling/disabling debugging and error logging.
    -   `json_logging.h`: Contains macros for debug prints and error logging.

### Changed

-   Updated the functionality to allow better control over debug and error logging, improving flexibility and performance in different environments.

---

## [1.0.0] - 2024-10-27

### Initial Release

-   Released the first official version of the JSON Parser Library.
-   Basic JSON parsing functionality implemented:
    -   Parsing of JSON objects and arrays.
    -   Supports strings, numbers, booleans, and null values.
    -   Easy-to-use API for accessing JSON data.
    -   Error handling for invalid JSON input.
-   Basic installation instructions and usage examples provided in the README.

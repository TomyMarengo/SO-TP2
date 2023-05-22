/**
 * @brief Initializes the stopwatch.
 */
void stw_init();

/**
 * @brief Updates the stopwatch.
 *
 * @param millis The amount of milliseconds elapsed since system startup.
 */
void stw_update(uint64_t millis);

/**
 * @brief Pauses the stopwatch if it is running or vice versa.
 *
 * @param millis The amount of milliseconds elapsed since system startup.
 */
void stw_changeStatus(uint64_t millis);

/**
 * @brief Stops the stopwatch.
 */
void stw_stop();
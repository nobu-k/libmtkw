#ifndef LIBMTKW_SRC_ENV_HPP_
#define LIBMTKW_SRC_ENV_HPP_

#include <string>

namespace mtkw {
namespace env {

/**
 * @brief Check if value is 'true'.
 *
 * @return If the value has one of valid 'true' values, this function
 * returns true. Otherwise, this function returns false.
 */
bool checkTrue(const std::string& value);

/**
 * @brief Get a bool value from the given environment variable.
 *
 * @param key The key of the target environment variable.
 * @param res The return value will be stored to this argument.
 *
 * @return If environment variable 'key' exists and it has non-empty
 * value, this function returns 0. Otherwise, this function returns -1.
 */
int getBoolean(const std::string& key, bool& res);

/**
 * @brief Get all flags of libmtkw.
 *
 * @param enabled The enabled-flag will be stored to this argument.
 * @param debug_mode The debug-flag will be stored to this argument.
 */
void getFlags(bool& enabled, bool& debug_mode);

/**
 * @brief Get enabled-flag.
 *
 * @param enabled The enabled-flag will be stored to this argument.
 *
 * @return If 'MTKW' or 'MTKW_ENABLE' environment variable exists and
 * it has non-empty value, this function returns 0 and stores the
 * enabled-flag to 'enabled' argument. Otherwise, this function returns -1.
 *
 * When this function returns -1, it does not change the value of
 * 'enabled' argument.
 */
int getEnabledFlag(bool& enabled);

/**
 * @note This function exceptional breaks the rule of argument order.
 * The output will be stored to the first argument.
 */
void getEnabledFlag(bool& enabled, bool default_value);

/**
 * @brief Get debug-flag.
 *
 * @param enabled The debug-flag will be stored to this argument.
 *
 * @return If 'MTKW_DEBUG' environment variable exists and
 * it has non-empty value, this function returns 0 and stores the
 * debug-flag to 'debug_mode' argument. Otherwise, this function returns -1.
 *
 * When this function returns -1, it does not change the value of
 * 'debug_mode' argument.
 */
int getDebugFlag(bool& debug_mode);

/**
 * @note This function exceptional breaks the rule of argument order.
 * The output will be stored to the first argument.
 */
void getDebugFlag(bool& debug_mode, bool default_value);

} // namespace env
} // namespace mtkw

#endif

#ifndef ELOGMACROS_H
#define ELOGMACROS_H

#include "Elog.h"

/** Macros for logging */

/** debug
 * @param logId the log id
 * @param message the message
 * @param ... the arguments
 */
#define LOG_D(logId, message, ...) logger.log(logId, DEBUG, message, ##__VA_ARGS__)

/** info
 * @param logId the log id
 * @param message the message
 * @param ... the arguments
 */
#define LOG_I(logId, message, ...) logger.log(logId, INFO, message, ##__VA_ARGS__)

/** notice
 * @param logId the log id
 * @param message the message
 * @param ... the arguments
 */
#define LOG_N(logId, message, ...) logger.log(logId, NOTICE, message, ##__VA_ARGS__)

/** warning
 * @param logId the log id
 * @param message the message
 * @param ... the arguments
 */
#define LOG_W(logId, message, ...) logger.log(logId, WARNING, message, ##__VA_ARGS__)

/** error
 * @param logId the log id
 * @param message the message
 * @param ... the arguments
 */
#define LOG_E(logId, message, ...) logger.log(logId, ERROR, message, ##__VA_ARGS__)

/** critical
 * @param logId the log id
 * @param message the message
 * @param ... the arguments
 */
#define LOG_C(logId, message, ...) logger.log(logId, CRITICAL, message, ##__VA_ARGS__)

/** alert
 * @param logId the log id
 * @param message the message
 * @param ... the arguments
 */
#define LOG_A(logId, message, ...) logger.log(logId, ALERT, message, ##__VA_ARGS__)

/** emergency
 * @param logId the log id
 * @param message the message
 * @param ... the arguments
 */
#define LOG_EM(logId, message, ...) logger.log(logId, EMERGENCY, message, ##__VA_ARGS__)

#endif // ELOGMACROS_H

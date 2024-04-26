#pragma once

#define QOSMETICS_WALLS_EXPORT __attribute__((visibility("default")))

#ifdef __cplusplus
#define QOSMETICS_WALLS_EXPORT_FUNC extern "C" QOSMETICS_WALLS_EXPORT
#else
#define QOSMETICS_WALLS_EXPORT_FUNC QOSMETICS_WALLS_EXPORT
#endif

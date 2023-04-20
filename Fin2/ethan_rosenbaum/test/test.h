#pragma once

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define msg_fmt(format, ...) (fprintf(stderr, "%s: " format "\n", __func__, __VA_ARGS__))
#define msg(str) msg_fmt(str "%s", "")
#define msg_errno(str) msg_fmt(str ": %s", strerror(errno))

#define expect_impl(expr, msg) ((expr) ? (int) 0 : (msg("expected " #expr), (int) 1))
#define expect(expr) expect_impl(expr, msg)
#define expect_errno(expr) expect_impl(expr, msg_errno)

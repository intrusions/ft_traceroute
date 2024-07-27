NAME         := ft_traceroute
CC           := gcc
SRCS_DIR     := srcs
OBJS_DIR     := .objs
INCS_DIR     := incs
MAIN         := srcs/main.c

CFLAGS :=                           \
    -Wall                           \
    -Wextra                         \
    -Werror                         \
    -pedantic                       
# export ASAN_OPTIONS="log_path=sanitizer.log"
# export ASAN_OPTIONS="detect_leaks=1"

CFLAGS_DBG :=                       \
    -g3                             \
    -O0                             \
    -fsanitize=address              \
    -fsanitize=undefined            \
    -fno-omit-frame-pointer         \
    -fstack-protector-strong        \
    -fno-optimize-sibling-calls 

SRCS :=                             \
    core/reverse_dns.c              \
    core/prepare_packet.c           \
    core/send_packet.c              \
    core/recv_packet.c              \
    core/initialization.c           \
    utils/calcul.c                  \
    utils/print.c                   \
    utils/manage_flags.c            \
    utils/checksum.c                \
    utils/close.c                   \
    utils/ip_to_hostname.c          \
    libc/mem_set.c                  \
    libc/str_cmp.c                  \
    libc/str_cpy.c                  \
    libc/str_len.c                  \
    libc/m_atoi.c                   \
    debug/debug_print.c                
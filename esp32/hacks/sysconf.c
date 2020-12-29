// HACK to add missing symbol
long sysconf(int _) {
    return 0;
}

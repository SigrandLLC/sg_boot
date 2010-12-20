void icache_invalidate_all(void);
void icahce_invalidate_line(void *va);
void icache_invalidate_block(void *va, int len);
void dcache_invalidate_all(void);
void dcache_invalidate_line(void *va);
void dcache_invalidate_block(void *va, int len);

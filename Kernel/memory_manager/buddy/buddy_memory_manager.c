// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memory_manager/buddy/buddy_memory_manager.h>

BuddyManager buddy_manager;

// Inicializar el gestor de memoria buddy
int8_t mm_init(void* p, uint32_t s) {
    int max_level = log2(s);
    if (max_level >= MAX_LEVELS)
        return -1;
    
    buddy_manager.memory_base = p;
    buddy_manager.memory_size = s;
    buddy_manager.used_memory = 0;

    memset(buddy_manager.free_lists, 0, sizeof(buddy_manager.free_lists));
    
    BlockHeader* initial_block = (BlockHeader*) buddy_manager.memory_base;
    initial_block->level = max_level;
    initial_block->free = 1;
    initial_block->next = NULL;

    buddy_manager.free_lists[max_level] = initial_block;

    return 0;
}

int get_level_for_size(uint32_t size) {
    int level = 0;
    uint32_t block_size = 1;
    while (block_size < size) {
        block_size *= 2;
        level++;
    }
    return level;
}

void split_block(int level) {
    if (buddy_manager.free_lists[level] == NULL) return;

    BlockHeader* block = buddy_manager.free_lists[level];
    buddy_manager.free_lists[level] = block->next;  // Elimino el bloque a dividir de su lista 

    int new_level = level - 1;
    uint32_t block_size = (1 << new_level);         // Equivalente a 2^new_level
    BlockHeader* buddy = (BlockHeader*)((uint8_t*)block + block_size);

    // Configuro los bloques partidos
    block->level = new_level;
    block->free = 1;

    buddy->level = new_level;
    buddy->free = 1;

    block->next = buddy_manager.free_lists[new_level];
    buddy_manager.free_lists[new_level] = block;

    buddy->next = buddy_manager.free_lists[new_level];
    buddy_manager.free_lists[new_level] = buddy;
}

uint8_t is_within_managed_memory(void* p) {
    return p >= buddy_manager.memory_base && 
           p < buddy_manager.memory_base + buddy_manager.memory_size;
}

void coalesce_block(BlockHeader* block) {
    uint32_t block_size = (1 << block->level);
    uint32_t offset = (uint8_t*)block - (uint8_t*)buddy_manager.memory_base;
    uint32_t buddy_offset = offset ^ block_size;
    BlockHeader* buddy = (BlockHeader*)((uint8_t*)buddy_manager.memory_base + buddy_offset);

    if (is_within_managed_memory(buddy) && buddy->level == block->level && buddy->free) {
        // Eliminar al buddy de la lista libre de su nivel
        BlockHeader** free_list = &buddy_manager.free_lists[block->level];
        BlockHeader* prev = NULL;
        while (*free_list != NULL) {
            if (*free_list == buddy) {
                if (prev != NULL) {
                    prev->next = buddy->next;
                } else {
                    buddy_manager.free_lists[block->level] = buddy->next;
                }
                break;
            }
            prev = *free_list;
            free_list = &(*free_list)->next;
        }

        if (offset > buddy_offset) 
            block = buddy;
        
        block->level++;
        coalesce_block(block);
    } else {
        block->next = buddy_manager.free_lists[block->level];
        buddy_manager.free_lists[block->level] = block;
    }
}

void* mm_malloc(uint32_t size) {
    int level = get_level_for_size(size + sizeof(BlockHeader));
    int current_level = level;

    while (current_level < MAX_LEVELS && buddy_manager.free_lists[current_level] == NULL) 
        current_level++;
    

    if (current_level == MAX_LEVELS) return NULL;

    while (current_level > level) {
        split_block(current_level);
        current_level--;
    }

    BlockHeader* block = buddy_manager.free_lists[level];
    buddy_manager.free_lists[level] = block->next;
    block->free = 0;
    buddy_manager.used_memory += (1 << level);       // Sumo 2^level a la memoria usada
    return (void*)(block + 1);
}

// Liberar memoria
void mm_free(void* p) {
    if (p == NULL || !is_within_managed_memory(p)) return;
    BlockHeader* block = (BlockHeader*)p - 1;
    if (!block->free) { 
        block->free = 1;
        buddy_manager.used_memory -= (1 << block->level);
        coalesce_block(block);
    }
}

uint32_t mm_get_total_memory() {
    return buddy_manager.memory_size;
}

uint32_t mm_get_used_memory() {
    return buddy_manager.used_memory;
}

uint32_t mm_get_free_memory() {
    return buddy_manager.memory_size - buddy_manager.used_memory;
}
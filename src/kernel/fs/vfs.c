#include "vfs.h"

/* The root of the filesystem. */
fs_node_t *fs_root = 0;

uint32 read_fs (fs_node_t *node, uint32 offset, uint32 size, uint8 *buffer)
{
    /* Has the node got a read callback? */
    if (node->read != 0)
      {
        return node->read (node, offset, size, buffer);
      }
    else
      {
        return 0;
      }
}

uint32 write_fs (fs_node_t *node, uint32 offset, uint32 size, uint8 *buffer)
{
    /* Has the node got a write callback? */
    if (node->write != 0)
      {
        return node->write (node, offset, size, buffer);
      }
    else
      {
        return 0;
      }
}

void open_fs (fs_node_t *node, uint8 read, uint8 write)
{
    /* Has the node got a open callback? */
    if (node->open != 0)
      {
        return node->open (node);
      }
}

void close_fs (fs_node_t *node)
{
    /* Has the node got a close callback? */
    if (node->close != 0)
      {
        return node->close (node);
      }
}

struct dirent * readdir_fs (fs_node_t *node, uint32 index)
{
    /* Is the node a directory, and does it have a callback? */
    if ((node->flags & 0x7) == FS_DIRECTORY && node->readdir != 0)
      {
        return node->readdir (node, index);
      }
    else
      {
        return 0;
      }
}

fs_node_t * finddir_fs (fs_node_t *node, char *name)
{
    /* Is the node a directory, and does it have a callback? */
    if ((node->flags & 0x7) == FS_DIRECTORY && node->finddir != 0)
      {
        return node->finddir (node, name);
      }
    else
      {
        return 0;
      }
}

vfs_dir_t *vfs_make_dir(char *name) {
    vfs_dir_t *out = malloc(sizeof(vfs_dir_t));
    out->name = malloc(memlen(name));
    out->first_file = NULL;
    out->first_dir = NULL;
    out->sibling = NULL;
    out->parent = NULL;
    memcpy(out->name, name, memlen(name));
    return out;
}

vfs_file_t *vfs_make_file(char *name) {
    vfs_file_t *out = malloc(sizeof(vfs_file_t));
    out->name = malloc(memlen(name) + 1);
    out->sibling = NULL;
    out->parent = NULL;
    memcpy(out->name, name, memlen(name));
    out->name[memlen(name)] = 0;
    out->size = 0;
    return out;
}

void vfs_add_file(vfs_file_t *add, vfs_dir_t *dir) {
    if (dir->first_file == NULL) { 
        dir->first_file = add;
        add->parent = dir;
        return;
    }        
    vfs_file_t *file = dir->first_file;
    while (file->sibling != NULL) {
        file = file->sibling;
    }
    file->sibling = add;
    add->parent = dir;
}

void vfs_add_dir(vfs_dir_t *add, vfs_dir_t *dir) {
    if (dir->first_dir == NULL) { 
        dir->first_dir = add;
        add->parent = dir;
        return;
    }        
    vfs_dir_t *dir2 = dir->first_dir;
    while (dir2->sibling != NULL) {
        dir2 = dir2->sibling;
    }
    dir2->sibling = add;
    add->parent = dir;
}

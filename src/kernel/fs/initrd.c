#include "initrd.h"

initrd_header_t         *initrd_header;         /* The header. */
initrd_file_header_t    *file_headers;          /* The list of file headers. */
fs_node_t               *initrd_root;           /* Our root directory node */
fs_node_t               *initrd_dev;            /* We also add a directory node for /dev, so we can mount devfs later on. */
fs_node_t               *root_nodes;            /* List of file nodes. */
int                     nroot_nodes;            /* Number of file nodes. */

struct dirent dirent;

static uint32
initrd_read (fs_node_t *node, uint32 offset, uint32 size, uint8 *buffer)
{
    initrd_file_header_t header = file_headers[node->inode];
    
    if (offset > header.length)
      {
        return 0;
      }
    if (offset + size > header.length)
      {
        size = header.length - offset;
      }

    memcpy (buffer, (uint8 *) (header.offset + offset), size);

    return size;
}

static struct dirent *
initrd_readdir (fs_node_t *node, uint32 index)
{
    if (node == initrd_root && index == 0)
      {
          strcpy (dirent.name, "dev");
          dirent.name[3] = 0; /* Make sure the string is NULL-terminated */
          dirent.ino     = 0;
          return &dirent;
      }

    if (index - 1 >= nroot_nodes)
        return 0;

    strcpy (dirent.name, root_nodes[index - 1].name);
    dirent.name[strlen (root_nodes[index - 1].name)] = 0; /* Make sure that the string is NULL-terminated */
    dirent.ino = root_nodes[index - 1].inode;
    return &dirent;
}

static fs_node_t *
initrd_finddir (fs_node_t *node, char *name)
{
    if (node == initrd_root && !strcmp (name, "dev"))
        return initrd_dev;

    int counter;
    for (counter = 0; counter < nroot_nodes; counter++)
        if (!strcmp (name, root_nodes[counter].name))
            return &root_nodes[counter];

    return 0;
}

fs_node_t *
initialise_initrd (uint32 location)
{
    /* Initialise the main and file header pointers and populate the root directory. */
    initrd_header = (initrd_header_t *) location;
    file_headers  = (initrd_file_header_t *) (location + sizeof (initrd_header_t));

    /* Initialise the root directory */
    initrd_root = (fs_node_t *) malloc (sizeof (fs_node_t));
    strcpy (initrd_root->name, "initrd");

    /* Chain initialisattion to 0. */
    initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = 0;

    initrd_root->flags   = FS_DIRECTORY;
    initrd_root->read    = 0;
    initrd_root->write   = 0;
    initrd_root->open    = 0;
    initrd_root->close   = 0;
    initrd_root->readdir = &initrd_readdir;
    initrd_root->finddir = &initrd_finddir;
    initrd_root->ptr     = 0;
    initrd_root->impl    = 0;

    /* Initialise the /dev directory (required) */
    initrd_dev = (fs_node_t *) malloc (sizeof (fs_node_t));
    strcpy (initrd_dev->name, "dev");

    /* Chain initialisation to 0. */
    initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = initrd_dev->inode = initrd_dev->length = 0;

    initrd_dev->flags    = FS_DIRECTORY;
    initrd_dev->read     = 0;
    initrd_dev->write    = 0;
    initrd_dev->open     = 0;
    initrd_dev->close    = 0;
    initrd_dev->readdir  = &initrd_readdir;
    initrd_dev->finddir  = &initrd_finddir;
    initrd_dev->ptr      = 0;
    initrd_dev->impl     = 0;

    /* Allocate space for the files in the ramdisk */
    root_nodes  = (fs_node_t *) malloc (sizeof (fs_node_t) * initrd_header->nfiles);
    nroot_nodes = initrd_header->nfiles;

    /* Now make them files */
    int counter;
    for (counter = 0; counter < initrd_header->nfiles; counter++)
      {
          /* Edit the file's header - currently it holds the file offset
           * relative to the start of the ramdisk. We want it relative
           * to the start of memory.
           */
          file_headers[counter].offset += location;

          /* Create a new file node. */
          strcpy (root_nodes[counter].name, &file_headers[counter].name);

          root_nodes[counter].mask = root_nodes[counter].uid = root_nodes[counter].gid = 0;
          
          root_nodes[counter].length  = file_headers[counter].length;
          root_nodes[counter].inode   = counter;
          root_nodes[counter].flags   = FS_FILE;
          root_nodes[counter].read    = &initrd_read;
          root_nodes[counter].write   = 0;
          root_nodes[counter].readdir = 0;
          root_nodes[counter].finddir = 0;
          root_nodes[counter].open    = 0;
          root_nodes[counter].close   = 0;
          root_nodes[counter].impl    = 0;
      }

    return initrd_root;
}
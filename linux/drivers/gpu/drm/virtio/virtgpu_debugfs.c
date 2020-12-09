/*
 * Copyright (C) 2015 Red Hat, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <drm/drm_debugfs.h>
#include <drm/drm_file.h>

#include "virtgpu_drv.h"

static void virtio_add_bool(struct seq_file *m, const char *name,
				    bool value)
{
	seq_printf(m, "%-16s : %s\n", name, value ? "yes" : "no");
}

static void virtio_add_int(struct seq_file *m, const char *name,
				   int value)
{
	seq_printf(m, "%-16s : %d\n", name, value);
}

static int virtio_gpu_features(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct virtio_gpu_device *vgdev = node->minor->dev->dev_private;

	virtio_add_bool(m, "virgl", vgdev->has_virgl_3d);
	virtio_add_bool(m, "edid", vgdev->has_edid);
	virtio_add_bool(m, "indirect", vgdev->has_indirect);
	virtio_add_int(m, "cap sets", vgdev->num_capsets);
	virtio_add_int(m, "scanouts", vgdev->num_scanouts);
	return 0;
}

static int
virtio_gpu_debugfs_irq_info(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct virtio_gpu_device *vgdev = node->minor->dev->dev_private;

	seq_printf(m, "fence %llu %lld\n",
		   (u64)atomic64_read(&vgdev->fence_drv.last_seq),
		   vgdev->fence_drv.sync_seq);
	return 0;
}

static struct drm_info_list virtio_gpu_debugfs_list[] = {
	{ "virtio-gpu-features", virtio_gpu_features },
	{ "virtio-gpu-irq-fence", virtio_gpu_debugfs_irq_info, 0, NULL },
};

#define VIRTIO_GPU_DEBUGFS_ENTRIES ARRAY_SIZE(virtio_gpu_debugfs_list)

void
virtio_gpu_debugfs_init(struct drm_minor *minor)
{
	drm_debugfs_create_files(virtio_gpu_debugfs_list,
				 VIRTIO_GPU_DEBUGFS_ENTRIES,
				 minor->debugfs_root, minor);
}

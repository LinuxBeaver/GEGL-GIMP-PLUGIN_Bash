/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2023 demiboy/barefootliam, Liam Quin, liam at fromoldbooks dot org
 */

/* Spawn an external process given as pipeline property
 * Give it the input as standard input,
 * or as a named file via the path-in property
 * Read its output via the path-result property
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_file_path  (path_in, _("Temporary filename for pipeline to read"), NULL)
    description  (_("Full path to a file that will be created containing the image for the pipeline to process"))

property_file_path  (path_out, _("Temporary filename for pipeline to create"), NULL)
    description  (_("The pipeline should create this file and leave an image in it, in the same format as the input file"))

property_string  (pipeline, _("An external system command to run"), NULL)
    description  (_("Pipeline to run with bash. Beware of filenames with spaces and of special characters."))

#else

#define GEGL_OP_AREA_FILTER
#define GEGL_OP_NAME     spawn
#define GEGL_OP_C_SOURCE spawn.c

#include "gegl-op.h"
#ifndef STDERR
# include <stdio.h>
#endif

static gboolean
save(GeglBuffer          *input,
     const GeglRectangle *roi,
     gchar *filename)
{
  GeglNode   *graph;
  GeglNode   *source;
  GeglNode   *writer;

  graph  = gegl_node_new ();

  source = gegl_node_new_child(graph,
      "operation", "gegl:buffer-source",
      "buffer", input,
      NULL);

  writer = gegl_node_new_child(graph,
      "operation", "gegl:png-save",
      "path", filename,
      NULL);

  gegl_node_link_many(source, writer, NULL);
  gegl_node_process(writer);
  g_object_unref(graph);
  return TRUE; /* how do we know if it worked? */
}

static void
load(gchar *filename, GeglBuffer **into)
{
  GeglNode   *graph = gegl_node_new();
  GeglNode   *src   = gegl_node_new_child(graph,
      "operation", "gegl:load",
      "path", filename,
      NULL);
  GeglNode   *sink  = gegl_node_new_child (graph,
                               "operation", "gegl:write-buffer",
                               "buffer", *into,
                                NULL);
  gegl_node_link_many (src, sink, NULL);

  gegl_node_process (sink);
  g_object_unref (graph);
}

static gboolean
process (GeglOperation       *operation,
         GeglBuffer          *input,
         GeglBuffer          *output,
         const GeglRectangle *roi,
         gint                 level)
{
  GeglProperties *o = GEGL_PROPERTIES(operation);
  /*
  GeglOperationAreaFilter *op_area   = GEGL_OPERATION_AREA_FILTER (operation);
  const Babl *format = gegl_operation_get_format (operation, "output");
  */

  if (o->pipeline) {
      if (o->path_in) {
	gchar *path = o->path_in;
	  if (! save(input, roi, path)) {
	    fprintf(stderr, "save failed to %s\n", path);
	    return FALSE;
	  }
      }
      system(o->pipeline);

      if (o->path_out) {
	/* not sure if i need to copy the output buffer instead? probably */
	load(o->path_out, &output);
      }
  } else {
    // fprintf(stderr, "process with no pieline ignored\n");
  }

  return TRUE;
}

static GeglRectangle
get_bounding_box (GeglOperation *operation)
{
  GeglRectangle  result = {0,0,1,4};
  GeglRectangle *in_rect;

  in_rect = gegl_operation_source_get_bounding_box (operation, "input");

  if (in_rect) {
    result = *in_rect;
  } else {
    // fprintf(stderr, "spawn: get_bounding_box() with null input\n");
  }
  
  return result;
} 

static void
prepare(GeglOperation *operation)
{
  const Babl *space   = gegl_operation_get_source_space (operation, "input");

 gegl_operation_set_format (operation, "input", babl_format_with_space ("RGBA float", space));
  gegl_operation_set_format (operation, "output", babl_format_with_space ("RGBA float", space));
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
  GeglOperationFilterClass *filter_class;

  operation_class = GEGL_OPERATION_CLASS (klass);
  filter_class = GEGL_OPERATION_FILTER_CLASS (klass);

  filter_class->process = process;
  operation_class->prepare = prepare;
  operation_class->threaded = FALSE;
  operation_class->get_bounding_box = get_bounding_box;

  gegl_operation_class_set_keys (operation_class,
    "name",        "boy:spawn",
    "title",       _("Run a system command"),
    "categories",  "generic",
    "license",     "GPL3+",
    "description", _("Run a system command using bash. Caution: make sure you trust the command."),
    NULL);
}


#endif

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
 * 2023 demiboy/barefootliam, Liam Quin, liam at fromoldbooks dot org (gegl boy:spawn)
 * 2023 Beaver, GEGL Bash Buddy (lb:bash) - Modification of Liam's boy:spawn plugin to make it easier.
 */

/* 
Recreation in GEGL graph (requires Liam's bash plugin that it does ship with)


boy:spawn path-in="/tmp/in.png" path-out="/tmp/out.png" pipeline="whatever bash string you want to put here"
gegl-buffer-load        
id=1 clear aux=[ ref=1 ]
layer src="/tmp/out.png"
gegl:gegl string='whatever gegl command you want to put here'
*/

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


/* output extent is an instruction to hide these sliders */
property_file_path  (pathin, _("Temporary filename for pipeline to read"),  "/tmp/in.png")
    ui_meta     ("role", "output-extent")


property_file_path  (pathout, _("Temporary filename for pipeline to create"), "/tmp/out.png") 
    ui_meta     ("role", "output-extent")


enum_start (image_call_policy_meme)
  enum_value (disableimage,      "noimage",
              N_("Load nothing but GEGL syntax"))
  enum_value (enableimage,      "previousimage",
              N_("Load /tmp/out.png"))
enum_end (imagecallpolicymeme)


property_enum (imagecallpolicy, _("Switch to final output?"),
    imagecallpolicymeme, image_call_policy_meme,
    enableimage)
    description (_("If this is enabled layer src= will always call  /tmp/out.png - If it is disabled it will not and only load GEGL syntax. Enabling this has advantages as you can run GEGL commands on the final bash output without reloading the bash checkbox each time. But it will also create a anomalous bug sometimes with GEGL Pixel Chunks and always load the previous file in /tmp/. The previous file being loaded is not a bug but the afromentioned thing is. By default this is enabled."))
  ui_meta     ("sensitive", "! bashtime")
 
property_string (bash, _("Insert Bash command that requires /tmp/out.png to be the final output"), bashwithanything)
  description    (_("This is the same as running bash in a terminal so be careful. /tmp/in.png is the original layer exported and /tmp/out.png  is the final output that GEGL imports. You can do advance techniques like use GMIC to convert the png to another format then run GMIC on that said format before porting it back to /tmp/out.png for the best results on certain GMIC filters, but the final output will always be /tmp/out.png - that is the file GEGL is told to import. Consider using this plugin to call GMIC, Image Magick, AI's like REMBG, GFPGAN or literally any image modifying task that works through bash. "))
    ui_meta ("multiline", "true")


#define bashwithanything \
"rembg /tmp/in.png -o /tmp/out.png"

property_boolean (bashtime, _("Run Bash Command (will freeze Gimp). Please disable this within seconds after enabling it. "), FALSE)
  description    (_("If done correct Gimp will freeze when this is checked. Then it is the users job to judge when they think the operation is done and uncheck this. Usually this happens instantly to a several seconds at most. But it will always be unique to the users machine specs and what bash command they run. After that select `load final output` in the drop down list. GMIC will be fast but an AI may take some time. If you don't (check, wait then uncheck) this the command, then it  will work but take dozens of times longer to load. You can tell how long it takes based on how long said bash command takes to input and output a file on your machine."))


property_string (geglsyntax, _("Optional GEGL Syntax after the bash command "), geglwithanything)
  description    (_(" This is optional. Input whatever GEGL syntax you want to run after the bash command. Perhaps edge smoothing operations and an outline to run after a tool like REMBG "))
    ui_meta ("multiline", "true")


#define geglwithanything \
"   "

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     bash_buddy
#define GEGL_OP_C_SOURCE bash-buddy.c

#include "gegl-op.h"


typedef struct
{
  GeglNode *input;
  GeglNode *bash;
  GeglNode *layercall;
  GeglNode *layercall2;
  GeglNode *nop;
  GeglNode *geglwhatever;

  GeglNode *output;
} State; 

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);


  State *state = o->user_data = g_malloc0 (sizeof (State));


  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");


  state->bash    = gegl_node_new_child (gegl,
                                  "operation", "boy:spawn",
                                  NULL);

  state->nop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  state->geglwhatever    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", 
                                  NULL);

#define lastimport \
"  src aux=[ layer src='/tmp/out.png'  "


  state->layercall    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", lastimport,
                                  NULL);

  state->layercall2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", lastimport,
                                  NULL);

      gegl_operation_meta_redirect (operation, "bash", state->bash, "pipeline");
      gegl_operation_meta_redirect (operation, "pathin", state->bash, "path-in");
      gegl_operation_meta_redirect (operation, "pathout", state->bash, "path-out");
      gegl_operation_meta_redirect (operation, "geglsyntax", state->geglwhatever, "string");


} 

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

if (o->bashtime) 


switch (o->imagecallpolicy) {
        break;
    case enableimage:
    gegl_node_link_many (state->input, state->bash, state->layercall2, state->nop,  state->geglwhatever,  state->output, NULL);
        break;
    case disableimage:
    gegl_node_link_many (state->input, state->bash, state->layercall2, state->nop,  state->geglwhatever,  state->output, NULL);
    }

else 

switch (o->imagecallpolicy) {
        break;
    case enableimage:
    gegl_node_link_many (state->input,  state->layercall, state->geglwhatever, state->output,  NULL);
        break;
    case disableimage:
    gegl_node_link_many (state->input,  state->geglwhatever, state->output, NULL);
}
    }
      

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class = GEGL_OPERATION_CLASS (klass);
   GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:bash",
    "title",       _("Bash Plugin"),
    "reference-hash", "235gmelaniemartinezismycutepsychogirlwaifu232344g3",
    "description", _("To use input a image modifying bash string that involves  dirnames /tmp/in.png and /tmp/out.png , then enable the checkbox. Gimp will freeze if done correct. Wait for under a second to a few seconds  then uncheck the checkbox to see if your final output is done. If you don't uncheck and recheck the checkbox this filter will work but load dozens of times slower. "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Bash in GEGL/",
    "gimp:menu-label", _("GEGL Bash..."),
    NULL);
}

#endif

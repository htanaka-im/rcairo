/* -*- c-file-style: "gnu"; indent-tabs-mode: nil -*- */
/*
 * Ruby Cairo Binding
 *
 * $Author: kou $
 * $Date: 2007-03-06 12:17:34 $
 *
 * Copyright 2005 Øyvind Kolås <pippin@freedesktop.org>
 * Copyright 2004-2005 MenTaLguY <mental@rydia.com>
 *
 * This file is made available under the same terms as Ruby
 *
*/


#include "rb_cairo.h"

VALUE rb_cCairo_FontFace;

#define _SELF  (RVAL2CRFONTFACE(self))

cairo_font_face_t *
rb_cairo_font_face_from_ruby_object (VALUE obj)
{
  cairo_font_face_t *face;
  if (!RTEST (rb_obj_is_kind_of (obj, rb_cCairo_FontFace)))
    {
      rb_raise (rb_eTypeError, "not a cairo font face");
    }
  Data_Get_Struct (obj, cairo_font_face_t, face);
  return face;
}

VALUE
rb_cairo_font_face_to_ruby_object (cairo_font_face_t *face)
{
  if (face)
    {
      cairo_font_face_reference (face);
      return Data_Wrap_Struct (rb_cCairo_FontFace, NULL,
                               cairo_font_face_destroy, face);
    }
  else
    {
      return Qnil;
    }
}

static VALUE
cr_font_face_get_type (VALUE self)
{
  return INT2NUM ( cairo_font_face_get_type (_SELF));
}


void
Init_cairo_font (void)
{
  rb_cCairo_FontFace =
    rb_define_class_under (rb_mCairo, "FontFace", rb_cObject);

  rb_define_method (rb_cCairo_FontFace, "type", cr_font_face_get_type, 0);
}
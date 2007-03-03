/* -*- c-file-style: "gnu"; indent-tabs-mode: nil -*- */
/*
 * Ruby Cairo Binding
 *
 * $Author: kou $
 * $Date: 2007-03-03 13:53:29 $
 *
 * Copyright 2005 Kouhei Sutou <kou@cozmixng.org>
 *
 * This file is made available under the same terms as Ruby
 *
*/


#include "rb_cairo.h"
#include "rb_cairo_private.h"

VALUE rb_cCairo_ScaledFont;

#define _SELF(self)  (RVAL2CRSCALEDFONT(self))

static inline void
cr_scaled_font_check_status (cairo_scaled_font_t *font)
{
  rb_cairo_check_status (cairo_scaled_font_status (font));
}

cairo_scaled_font_t *
rb_cairo_scaled_font_from_ruby_object (VALUE obj)
{
  cairo_scaled_font_t *font;
  if (!RTEST (rb_obj_is_kind_of (obj, rb_cCairo_ScaledFont)))
    {
      rb_raise (rb_eTypeError, "not a cairo scaled font");
    }
  Data_Get_Struct (obj, cairo_scaled_font_t, font);
  return font;
}

static void
cr_scaled_font_free (void *ptr)
{
  if (ptr)
    {
      cairo_scaled_font_destroy ((cairo_scaled_font_t *) ptr);
    }
}

VALUE
rb_cairo_scaled_font_to_ruby_object (cairo_scaled_font_t *font)
{
  if (font)
    {
      cairo_scaled_font_reference (font);
      return Data_Wrap_Struct (rb_cCairo_ScaledFont, NULL,
                               cr_scaled_font_free, font);
    }
  else
    {
      return Qnil;
    }
}

static VALUE
cr_scaled_font_allocate (VALUE klass)
{
  return Data_Wrap_Struct (klass, NULL, cr_scaled_font_free, NULL);
}

static VALUE
cr_scaled_font_initialize (VALUE self, VALUE face, VALUE matrix,
                           VALUE ctm, VALUE options)
{
  cairo_scaled_font_t *font;

  font = cairo_scaled_font_create (RVAL2CRFONTFACE (face),
                                   RVAL2CRMATRIX (matrix),
                                   RVAL2CRMATRIX (ctm),
                                   RVAL2CRFONTOPTIONS (options));
  cr_scaled_font_check_status (font);
  DATA_PTR (self) = font;
  return Qnil;
}

static VALUE
cr_scaled_font_get_type (VALUE self)
{
  return INT2NUM (cairo_scaled_font_get_type ( _SELF (self)));
}

static VALUE
cr_scaled_font_extents (VALUE self)
{
  cairo_font_extents_t extents;
  cairo_scaled_font_extents (_SELF (self), &extents);
  cr_scaled_font_check_status (_SELF (self));
  return CRFONTEXTENTS2RVAL (&extents);
}

static VALUE
cr_scaled_font_text_extents (VALUE self, VALUE utf8)
{
  cairo_text_extents_t extents;
  cairo_scaled_font_text_extents (_SELF (self), StringValueCStr (utf8),
                                  &extents);
  cr_scaled_font_check_status (_SELF (self));
  return CRTEXTEXTENTS2RVAL (&extents);
}

static VALUE
cr_scaled_font_glyph_extents (VALUE self, VALUE rb_glyphs)
{
  cairo_text_extents_t extents;
  cairo_glyph_t *glyphs;
  int count;

  rb_cairo__glyphs_to_array (rb_glyphs, &glyphs, &count);
  cairo_scaled_font_glyph_extents (_SELF (self), glyphs, count, &extents);
  cr_scaled_font_check_status (_SELF (self));
  return CRTEXTEXTENTS2RVAL (&extents);
}

static VALUE
cr_scaled_font_get_font_face (VALUE self)
{
  cairo_font_face_t *face;
  face = cairo_scaled_font_get_font_face (_SELF (self));
  cr_scaled_font_check_status (_SELF (self));
  return CRFONTFACE2RVAL (face);
}

static VALUE
cr_scaled_font_get_font_matrix (VALUE self)
{
  cairo_matrix_t font_matrix;
  cairo_scaled_font_get_font_matrix (_SELF (self), &font_matrix);
  cr_scaled_font_check_status (_SELF (self));
  return CRMATRIX2RVAL (&font_matrix);
}

static VALUE
cr_scaled_font_get_ctm (VALUE self)
{
  cairo_matrix_t ctm;
  cairo_scaled_font_get_font_matrix (_SELF (self), &ctm);
  cr_scaled_font_check_status (_SELF (self));
  return CRMATRIX2RVAL (&ctm);
}

static VALUE
cr_scaled_font_get_font_options (VALUE self)
{
  cairo_font_options_t *options = cairo_font_options_create();
  cairo_scaled_font_get_font_options (_SELF (self), options);
  cr_scaled_font_check_status (_SELF (self));
  rb_cairo_check_status (cairo_font_options_status (options));
  return CRFONTOPTIONS2RVAL (options);
}

void
Init_cairo_scaled_font (void)
{
  rb_cCairo_ScaledFont =
    rb_define_class_under (rb_mCairo, "ScaledFont", rb_cObject);

  rb_define_alloc_func (rb_cCairo_ScaledFont, cr_scaled_font_allocate);

  rb_define_method (rb_cCairo_ScaledFont, "initialize",
                    cr_scaled_font_initialize, 4);

  rb_define_method (rb_cCairo_ScaledFont, "type", cr_scaled_font_get_type, 0);
  rb_define_method (rb_cCairo_ScaledFont, "extents", cr_scaled_font_extents, 0);
  rb_define_method (rb_cCairo_ScaledFont, "text_extents",
                    cr_scaled_font_text_extents, 1);
  rb_define_method (rb_cCairo_ScaledFont, "glyph_extents",
                    cr_scaled_font_glyph_extents, 1);
  rb_define_method (rb_cCairo_ScaledFont, "font_face",
                    cr_scaled_font_get_font_face, 0);
  rb_define_method (rb_cCairo_ScaledFont, "font_matrix",
                    cr_scaled_font_get_font_matrix, 0);
  rb_define_method (rb_cCairo_ScaledFont, "ctm", cr_scaled_font_get_ctm, 0);
  rb_define_method (rb_cCairo_ScaledFont, "font_options",
                    cr_scaled_font_get_font_options, 0);
}

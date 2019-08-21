/*************************************************************************/
/*  gdscript_extend_parser.h                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2019 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2019 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef GDSCRIPT_EXTEND_PARSER_H
#define GDSCRIPT_EXTEND_PARSER_H

#include "../gdscript_parser.h"
#include "core/variant.h"
#include "lsp.hpp"

#ifndef LINE_NUMBER_TO_INDEX
#define LINE_NUMBER_TO_INDEX(p_line) ((p_line)-1)
#endif

#ifndef SYMBOL_SEPERATOR
#define SYMBOL_SEPERATOR "::"
#endif

#ifndef JOIN_SYMBOLS
#define JOIN_SYMBOLS(p_path, name) ((p_path) + SYMBOL_SEPERATOR + (name))
#endif

typedef HashMap<String, const lsp::DocumentSymbol *> ClassMembers;

class ExtendGDScriptParser : public GDScriptParser {

	String path;
	Vector<String> lines;

	lsp::DocumentSymbol class_symbol;
	Vector<lsp::Diagnostic> diagnostics;
	ClassMembers members;
	HashMap<String, ClassMembers> inner_classes;

	void update_diagnostics();

	void update_symbols();
	void parse_class_symbol(const GDScriptParser::ClassNode *p_class, lsp::DocumentSymbol &r_symbol);
	void parse_function_symbol(const GDScriptParser::FunctionNode *p_func, lsp::DocumentSymbol &r_symbol);

	Dictionary dump_function_api(const GDScriptParser::FunctionNode *p_func) const;
	Dictionary dump_class_api(const GDScriptParser::ClassNode *p_class) const;

	String parse_documentation(int p_line, bool p_docs_down = false);
	const lsp::DocumentSymbol *search_symbol_defined_at_line(int p_line, const lsp::DocumentSymbol &p_parent) const;

	Array member_completions;

	String parse_documentation_as_markdown(int p_line, bool p_docs_down = false);

public:
	static String marked_documentation(const String &p_bbcode);

public:
	_FORCE_INLINE_ const String &get_path() const { return path; }
	_FORCE_INLINE_ const Vector<String> &get_lines() const { return lines; }
	_FORCE_INLINE_ const lsp::DocumentSymbol &get_symbols() const { return class_symbol; }
	_FORCE_INLINE_ const Vector<lsp::Diagnostic> &get_diagnostics() const { return diagnostics; }
	_FORCE_INLINE_ const ClassMembers &get_members() const { return members; }
	_FORCE_INLINE_ const HashMap<String, ClassMembers> &get_inner_classes() const { return inner_classes; }

	String get_text_for_completion(const lsp::Position &p_cursor) const;
	int get_parameter_count(const lsp::Position &p_cursor, lsp::Position *p_begining_position);
	void get_document_link(List<lsp::DocumentLink> *r_links_parameter);
	String get_text_for_lookup_symbol(const lsp::Position &p_cursor, const String &p_symbol = "", bool p_func_requred = false) const;
	String get_identifier_under_position(const lsp::Position &p_position, Vector2i &p_offset) const;
	String get_uri() const;

	const lsp::DocumentSymbol *get_symbol_defined_at_line(int p_line) const;
	const lsp::DocumentSymbol *get_member_symbol(const String &p_name, const String &p_subclass = "") const;

	const Array &get_member_completions();
	Dictionary generate_api() const;

	Error parse(const String &p_code, const String &p_path);
};

#endif

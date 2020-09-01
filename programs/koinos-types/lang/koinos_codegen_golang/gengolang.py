
try:
    import importlib.resources as pkg_resources
except ImportError:
    # Try backported to PY<37 `importlib_resources`.
    import importlib_resources as pkg_resources

#from . import templates  # relative-import the *package* containing the templates

import jinja2

import collections
import os

class RenderError(Exception):
    pass

def classname_case_gen(name):
    it = iter(name)
    for c in it:
        if c != "_":
            yield c.lower()
            break
    for c in it:
        yield c.lower()

def classname_case(name):
    return "".join(classname_case_gen(name))

def simple_name(fqn):
    if isinstance(fqn, list):
        return fqn[-1]
    else:
        return fqn.split("::")[-1]

def fq_name(name):
    return "::".join(name)

def cpp_namespace(name):
    u = name.split("::")
    if len(u) <= 1:
        return ""
    return "::".join(u[:-1])

def cpp_classname(current_ns, name):
    name_ns = "::".join(name[:-1])
    if name_ns == current_ns:
        return name[-1]
    return "::".join(name)

def typeref_has_type_parameters(tref):
    # Typedef with type parameters is a base typedef.
    if tref["targs"] is None:
        return False
    for targ in tref["targs"]:
        if "value" not in targ:
            return True
    return False

def template_raise(cause):
    # Helper function to allow templates to raise exceptions
    # See https://stackoverflow.com/questions/21778252/how-to-raise-an-exception-in-a-jinja2-macro
    raise RenderError(cause)

def generate_golang(schema):
    env = jinja2.Environment(
            loader=jinja2.PackageLoader(__package__, "templates"),
            keep_trailing_newline=True,
        )
    env.filters["classname_case"] = classname_case
    env.filters["simple_name"] = simple_name
    env.filters["typeref_has_type_parameters"] = typeref_has_type_parameters
    env.filters["fq_name"] = fq_name
    env.filters["tuple"] = tuple
    env.filters["cpp_namespace"] = cpp_namespace
    decls_by_name = collections.OrderedDict(((fq_name(name), decl) for name, decl in schema["decls"]))
    decl_namespaces = sorted(set(cpp_namespace(name) for name in decls_by_name))

    env.globals["raise"] = template_raise
    env.globals["cpp_classname"] = cpp_classname

    ctx = {"schema" : schema,
           "decls_by_name" : decls_by_name,
           "decl_namespaces" : decl_namespaces,
          }
    for name, val in ctx["decls_by_name"].items():
        print(name)
        import json
        print(json.dumps(val))

    result = collections.OrderedDict()
    result_files = collections.OrderedDict()
    result["files"] = result_files

    template_names = [
        #"classes.hpp.j2",
        #"thunk_ids.hpp.j2",
        #"system_call_ids.hpp.j2",
        #"thunk_ids.h.j2",
        #"system_call_ids.h.j2",
        ]

    for template_name in template_names:
        j2_template = env.get_template(template_name)
        out_filename = os.path.splitext(template_name)[0]
        result_files[out_filename] = j2_template.render(ctx)

    rt_path = os.path.join(os.path.dirname(__file__), "rt")
    for root, dirs, files in os.walk(rt_path):
        for f in files:
            filepath = os.path.join(root, f)
            relpath = os.path.relpath(filepath, rt_path)
            with open(filepath, "r") as f:
                content = f.read()
            result_files[os.path.join("rt", relpath)] = content
    return result

def setup(app):
    app.register_target("golang", generate_golang)

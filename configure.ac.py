#!/usr/bin/env python3

from lib2to3.pytree import convert
import toml

toml_config_file_name = 'config.toml'

def convert_strings(value):
    if isinstance(value, str):
        return f'[{value}]'
    elif isinstance(value, list):
        return list(map(convert_strings, value))
    elif isinstance(value, dict):
        return Config(**value)
    else:
        return value


class Config:
    def __init__(self, **entries):
        for (key, value) in entries.items():
            entries[key] = convert_strings(value)
        
        self.__dict__.update(entries)
        
        
    def __repr__(self):
        return '<%s>' % str(' '.join('%s : %s' % (k, repr(v)) for (k, v) in self.__dict__.items())) 


def main():
    config = Config(**toml.load(toml_config_file_name))

    content = ''
    with open('configure.scan', 'r') as configure_scan:
        content = configure_scan.read()
    
    content = content.replace("[FULL-PACKAGE-NAME]", config.package.name)
    content = content.replace("[VERSION]", config.package.version)
    content = content.replace("[BUG-REPORT-ADDRESS]", config.package.email)
    content = content.replace("AC_PROG_CC", "AC_PROG_CC\nAM_INIT_AUTOMAKE")
    content = content.replace("# Checks for header files.", f"# Checks for header files.\nAC_CHECK_HEADERS({', '.join(config.package.headers)})")
    content = content.replace("# Checks for libraries.", "# Checks for libraries.\n" + 
    '\n'.join(
        map(lambda lib: f'AC_CHECK_LIB({lib.name}, {lib.function}, [], [AC_MSG_ERROR([Unable to find lib{lib.name.replace("[", "").replace("]", "")}])])', config.libraries)
        )
    )

    with open('configure.ac', 'w') as configure_ac:
        configure_ac.write(content)

    return 0


if __name__ == '__main__':
    exit_code = main()
    exit(exit_code)
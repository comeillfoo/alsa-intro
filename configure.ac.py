#!/usr/bin/env python3

import toml

toml_config_file_name = 'config.toml'


class Config:
    def __init__(self, **entries):
        for (key, value) in entries.items():
            if isinstance(value, str):
                entries[key] = f'[{value}]'
        
        self.__dict__.update(entries)
        
        
    def __repr__(self):
        return '<%s>' % str(' '.join('%s : %s' % (k, repr(v)) for (k, v) in self.__dict__.items())) 


def main():
    config = Config(**toml.load(toml_config_file_name))
    config.package = Config(**config.package)

    content = ''
    with open('configure.scan', 'r') as configure_scan:
        content = configure_scan.read()
    
    content = content.replace("[FULL-PACKAGE-NAME]", config.package.name)
    content = content.replace("[VERSION]", config.package.version)
    content = content.replace("[BUG-REPORT-ADDRESS]", config.package.email)
    content = content.replace("AC_PROG_CC", "AC_PROG_CC\nAM_INIT_AUTOMAKE")

    with open('configure.ac', 'w') as configure_ac:
        configure_ac.write(content)


if __name__ == '__main__':
    main()
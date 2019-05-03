import re
import os

name = 'Rule'
filename = 'src/frontend/rules/rules.hpp'

with open(filename, 'r') as f:
    content = f.read()

# content = ""
    
content = re.sub("{", ' {', content)
content = re.sub("\s+", ' ', content)


regexp = re.compile(r'enum( class)? ' + '([^{]*)' + ' \{([^}]*)\}', re.DOTALL|re.U)

# found = regexp.search(content).groups()

outputs = []

for found in regexp.findall(content):
    print(regexp.findall(content))

    values = found[2]
    values = re.sub(' ', '', values).split(',')

    print(values)

    header = 'std::string to_string({} val)'.format(found[1]) + '''
{
#define RETURN_NAME(p) case(p): return std::string(#p);
    switch(val){
'''
    tail = '''
        default: throw std::invalid_argument(std::string("unhandled enum value in ") \\
                                              + __FILE__ + ", line " + std::to_string(__LINE__));
    }
#undef RETURN_NAME
}
'''
    def wrapper(word):
        return '{}RETURN_NAME({});'.format(' ' * 8, word)

    output = header + '\n'.join(wrapper(word) for word in values if word) + tail
    outputs.append(output)


print('\n'.join(outputs))
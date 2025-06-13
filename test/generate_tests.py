state_graph = {
    'STATE_START': [
        ('EVT_START_UP', 'STATE_NOT_READY')
    ],
    
    'STATE_NOT_READY': [
        ('EVT_COMMS_ON', 'STATE_SWITCH_DISABLED'),
        ('EVT_FAULT', 'STATE_FAULT_REAC')
    ],
    
    'STATE_SWITCH_DISABLED': [
        ('EVT_REC_SHUTDOWN', 'STATE_SWITCH_READY'),
        ('EVT_FAULT', 'STATE_FAULT_REAC')
    ],
    
    'STATE_SWITCH_READY': [
        ('EVT_REC_SWITCHON', 'STATE_SWITCH_ON'),
        ('EVT_REC_VOLTAGE_DISABLE', 'STATE_SWITCH_DISABLED'),
        ('EVT_FAULT', 'STATE_FAULT_REAC')
    ],
    
    'STATE_SWITCH_ON': [
        ('EVT_REC_OP_ENABLE', 'STATE_OP_ENABLED'),
        ('EVT_REC_SHUTDOWN', 'STATE_SWITCH_READY'),
        ('EVT_REC_VOLTAGE_DISABLE', 'STATE_SWITCH_DISABLED'),
        ('EVT_FAULT', 'STATE_FAULT_REAC')
    ],
    
    'STATE_OP_ENABLED': [
        ('EVT_REC_OP_DISABLE', 'STATE_SWITCH_ON'),
        ('EVT_REC_SHUTDOWN', 'STATE_SWITCH_READY'),
        ('EVT_REC_QUICKSTOP', 'STATE_QUICK_STOP'),
        ('EVT_REC_VOLTAGE_DISABLE', 'STATE_SWITCH_DISABLED'),
        ('EVT_FAULT', 'STATE_FAULT_REAC')
    ],
    
    'STATE_QUICK_STOP': [
        ('EVT_REC_VOLTAGE_DISABLE', 'STATE_SWITCH_DISABLED'),
        ('EVT_FAULT', 'STATE_FAULT_REAC')
    ],
    
    'STATE_FAULT_REAC': [
        ('EVT_FAULT_REAC', 'STATE_FAULT')
    ],
    
    'STATE_FAULT': [
        ('EVT_REC_FAULTRESET', 'STATE_SWITCH_DISABLED')
    ]
}


fsm_name = '_fsm'
header_name = 'test_trans.h'
src_name = 'test_trans.c'

def join_commands(commands: list[str]) -> str:
    return ';\n'.join(commands) + ';'

def set_state_wrap(state: str) -> str:
    return f'fsm_set_state({fsm_name}, {state})'

def queue_push_wrap(event: str) -> str:
    return f'queue_push(fsm_get_queue({fsm_name}), {event})'

def queue_handle_wrap() -> str:
    return f'fsm_handle_evt({fsm_name})'

def test_equal_wrap(expected: str, actual: str) -> str:
    return f'TEST_ASSERT_EQUAL({expected}, {actual})'

def test_not_equal_wrap(expected: str, actual: str) -> str:
    return f'TEST_ASSERT_NOT_EQUAL({expected}, {actual})'

def get_state_wrap() -> str:
    return f'fsm_get_state({fsm_name})'

def comment_wrap(line: str) -> str:
    return f'// {line}'

def state_generate_tests(state: str, transitions: tuple[str, str]) -> list[str]:
    output = []
    for event, new_state in transitions:
        output.append(f'fsm_purge({fsm_name})')
        output.append(set_state_wrap(state))
        output.append(queue_push_wrap(event))
        output.append(test_equal_wrap('FSM_ERR_OK', queue_handle_wrap()))
        output.append(test_equal_wrap(f'{new_state}', get_state_wrap()))
    return output

def tab_wrap(line: str, level: int) -> str:
    return '\t'*level + line

def func_wrap(name: str, body: list[str]) -> str:
    result = f'void {name}(void)'+' {\n'
    output = []
    for line in body:
        output.append(tab_wrap(line, 1))
    result += join_commands(output) + '\n}'
    return result

def func_proto_wrap(name: str) -> str:
    return f'void {name}(void)'

def run_test_wrap(name: str) -> str:
    return f'RUN_TEST({name})'

if __name__ == '__main__':
    func_protos, func_defs, test_lines = [], [], []
    gen_tests_alias = lambda s: state_generate_tests(s, state_graph[s])
    func_protos.append(func_proto_wrap('tests_fsm_trans_setup'))
    func_defs.append(func_wrap(f'tests_fsm_trans_setup', \
            [f'{fsm_name} = fsm_create()', f'fsm_start({fsm_name})']))

    for key in state_graph.keys():
        func_name = f'test_fsm_trans_{key.lower()}'
        func_protos.append(func_proto_wrap(func_name))
        func_defs.append(func_wrap(func_name, gen_tests_alias(key)))
        test_lines.append(run_test_wrap(func_name))
    
    func_protos.append(func_proto_wrap('test_fsm_trans_all'))
    func_defs.append(func_wrap('test_fsm_trans_all', test_lines))
    
    with open(f'./{header_name}', 'w+') as file:
        file.write('#pragma once\n')
        file.write('#include <unity.h>\n')
        file.write('#include \"fsm.h\"\n')
        file.write('#include \"evt_queue.h\"\n')
        file.write('\n')

        file.write(join_commands(func_protos))
    
    with open(f'./{src_name}', 'w+') as file:
        file.write('#include \"test_trans.h\"\n')
        file.write('\n')
        file.write(f'static fsm_t {fsm_name} = NULL;\n')
        file.write('\n')

        for func_def in func_defs:
            file.write(func_def)
            file.write('\n\n')
        

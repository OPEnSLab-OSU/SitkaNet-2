"{\
	'general':\
	{\
		'name':'Sitka',\
		'instance':4,\
		'interval':1000,\
		'print_verbosity':2\
	},\
	'components':[\
    {\
      'name':'Analog',\
      'params':[\
        4,\
        8,\
        false,\
        false,\
        false,\
        false,\
        false,\
        false,\
        0,\
        0,\
        0,\
        0,\
        0,\
        0,\
        25.0\
      ]\
    },\
		{\
			'name':'Multiplexer',\
			'params':[113,8,true, 5000]\
		},\
		{\
			'name':'SD',\
			'params': 'default'\
		},\
		{\
			'name':'DS3231',\
			'params':[\
        12,\
        false\
      ]\
		},\
    {\
      'name':'Interrupt_Manager',\
      'params':[0]\
    },\
    {\
      'name':'Sleep_Manager',\
      'params':[true,false,1]\
    },\
    {\
      'name':'LoRa',\
      'params':[\
        255,\
        4,\
        23,\
        3,\
        200\
      ]\
    }\
	]\
}"

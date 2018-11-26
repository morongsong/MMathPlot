

-- the lua test
project 'luatest'
    kind 'ConsoleApp'
    
	mr_curdir_build_copyfile('func.lua')   
    files {'lua_t1.cpp'}
	pm_lib_use_lua('static')

  
  project 'luatest2'
    kind 'ConsoleApp'
       
    files {'lua_t2.cpp','LCalc.*'}
    pm_lib_use_lua('static')


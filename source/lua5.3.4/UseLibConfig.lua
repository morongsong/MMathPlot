

lua_build_share = false
lua_build_static = true

lua_lib_dir = path.translate(path.getabsolute("."), "\\")

function pm_lib_use_lua(drtype)
    the_lib_name    = 'lua'
	the_lib_src_dir = lua_lib_dir
	the_lib_bin_dir = g_the_top_dir --the_lib_src_dir
	the_lib_share_defines = {'LUA_BUILD_AS_DLL'}
	the_lib_static_defines = {}

	includedirs { the_lib_src_dir }
    if drtype == "share" then
        links_lib_dir(the_lib_bin_dir,the_lib_name..'.lib')
        cp_dlllib_dir(the_lib_bin_dir,the_lib_name..'.dll')
        cp_dlllib_dir(the_lib_bin_dir,the_lib_name..'.pdb')
		for i = 1, #the_lib_share_defines do defines {the_lib_share_defines[i]} end  
        linkoptions {'/DELAYLOAD:'..the_lib_name..'.dll'}
		dependson(the_lib_name)	
	elseif drtype == "static" then   
        links_lib_dir(the_lib_bin_dir,the_lib_name..'Static.lib')
		for i = 1, #the_lib_static_defines do defines {the_lib_static_defines[i]} end  
		dependson(the_lib_name.."Static")
    end
end

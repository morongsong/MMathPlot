--
-- The Global of Solution Setting
-- Version 1.5.3
--

--the_sln_name
--the_start_prj
--g_the_top_dir
--g_use_pmakeprj

if not g_the_top_dir then
    g_the_top_dir = path.translate(path.getabsolute("."), "\\")
else
    return
end

the_vs_name  = ""
the_vc_ver  = ""

if _ACTION == "vs2008" then
    the_vc_ver = "vc9"
    the_vs_name = "vs9"
elseif _ACTION == "vs2010" then
    the_vc_ver = "vc10"
    the_vs_name = "vs10"
elseif _ACTION == "vs2012" then
    the_vc_ver = "vc11"
    the_vs_name = "vs11"
elseif _ACTION == "vs2013" then
    the_vc_ver = "vc12"
    the_vs_name = "vs12"
elseif _ACTION == "vs2015" then
    the_vc_ver = "vc14"
    the_vs_name = ""
elseif _ACTION == "vs2017" then
    the_vc_ver = "vc15"
    the_vs_name = "vc15"
end   


function mr_build_copyfile(srcfile,srcdir,destdir)
	--print(srcfile,srcdir,destdir)
    dest_with_file = path.translate(path.getabsolute(destdir..'/'..srcfile), "/")
	src_with_file = path.translate(path.getabsolute(srcdir..'/'..srcfile), "/")

	--print(dest_with_file)
	--print(src_with_file)
	if dest_with_file == src_with_file then
		return
	end

    destdirfx = path.translate(path.getabsolute(destdir), "/")
	os.mkdir(destdirfx)
	--print(destdirfx)

	bisfile = os.isfile(dest_with_file)

	if not bisfile then
		ok, err = os.copyfile(src_with_file,dest_with_file)
		if ok then
			print('cp file '..srcfile..' from '..srcdir..' to '..destdirfx..'  ok...')
		else
			print('cp file '..srcfile..' from '..srcdir..' to '..destdirfx..'  fail 1...')
		end
	else
		info = os.stat(dest_with_file)
		info2 = os.stat(src_with_file)
		--print(info,info2)
		if info and info2 then
			if (info.mtime==info2.mtime) and (info.size==info2.size) then
				--print('cp file '..srcfile..' from '..srcdir..' to '..destdirfx..'  same file ok...')
			else
				ok, err = os.copyfile(src_with_file,dest_with_file)
				if ok then
					print('cp file '..srcfile..' from '..srcdir..' to '..destdirfx..'  ok...')
				else
					print('cp file '..srcfile..' from '..srcdir..' to '..destdirfx..'  fail 2...')
				end
			end
		else
			print('cp file '..srcfile..' from '..srcdir..' to '..destdir..'  fail 3...')
		end
	end
end

function mr_curdir_build_copyfile(srcfile)
	mr_build_copyfile(srcfile,'./',g_the_top_dir..'/Bin/Debug')
	mr_build_copyfile(srcfile,'./',g_the_top_dir..'/Bin/Release')
	mr_build_copyfile(srcfile,'./',g_the_top_dir..'/Bin/Debugx64')
	mr_build_copyfile(srcfile,'./',g_the_top_dir..'/Bin/Releasex64')
end

function links_lib_dir(UpLinkDir,LibName)
	filter {}
	filter {"platforms:Win32","configurations:Debug"}
		links{UpLinkDir..'/Lib'..the_vs_name..'/Debug/'..LibName}
	filter {"platforms:Win32","configurations:Release"}
		links{UpLinkDir..'/Lib'..the_vs_name..'/Release/'..LibName}

	filter {"platforms:x64","configurations:Debug"}
		links{UpLinkDir..'/Lib'..the_vs_name..'/Debugx64/'..LibName}
	filter {"platforms:x64","configurations:Release"}
		links{UpLinkDir..'/Lib'..the_vs_name..'/Releasex64/'..LibName}
	filter {}
end

function cp_dlllib_dir(UpLinkDir,LibName)
	filter {}

	filter {"platforms:Win32","configurations:Debug"}
        the_bin_dir=UpLinkDir..'/Bin'..the_vs_name..'/Debug/'
        mr_build_copyfile(LibName,the_bin_dir,g_the_top_dir..'/Bin/Debug')
	filter {"platforms:Win32","configurations:Release"}
        the_bin_dir=UpLinkDir..'/Bin'..the_vs_name..'/Release/'
        mr_build_copyfile(LibName,the_bin_dir,g_the_top_dir..'/Bin/Release')

	filter {"platforms:x64","configurations:Debug"}
        the_bin_dir=UpLinkDir..'/Bin'..the_vs_name..'/Debugx64/'
        mr_build_copyfile(LibName,the_bin_dir,g_the_top_dir..'/Bin/Debugx64')
	filter {"platforms:x64","configurations:Release"}
        the_bin_dir=UpLinkDir..'/Bin'..the_vs_name..'/Releasex64/'
        mr_build_copyfile(LibName,the_bin_dir,g_the_top_dir..'/Bin/Releasex64')
        
	filter {}
end

pm_lib_use_fun = {}
function pm_lib_use(libname,usetype)
	local fn = pm_lib_use_fun[libname]
	if fn then
		fn(usetype)
	end
end
    
workspace (the_sln_name)
    configurations {'Debug', 'Release'}
    platforms {"Win32","x64"}
    location ('_Build'..the_vs_name)
	characterset ('Unicode')

	if the_start_prj then
		startproject (the_start_prj)
	end

	filter {"language:C++"}
		links{'DelayImp.lib'}
		--defines {'_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS'}

	filter "platforms:Win32"
		architecture 'x32'
	filter "platforms:x64"
		architecture 'x64'
        

	filter {"platforms:Win32"}
		targetdir ("Bin"..the_vs_name.."/%{cfg.buildcfg}")
		implibdir ("Lib"..the_vs_name.."/%{cfg.buildcfg}")
		debugdir  ("Bin"..the_vs_name.."/%{cfg.buildcfg}")
	filter {"platforms:x64"}
		targetdir ("Bin"..the_vs_name.."/%{cfg.buildcfg}x64")
		implibdir ("Lib"..the_vs_name.."/%{cfg.buildcfg}x64")
		debugdir  ("Bin"..the_vs_name.."/%{cfg.buildcfg}x64")

	filter {"configurations:Debug"}
		defines { "DEBUG","_DEBUG"}
		symbols "On"
	filter {"configurations:Release"}	
		defines { "NDEBUG" }
		optimize "On"
		symbols "On"
        
    filter {'kind:StaticLib',"platforms:Win32"}
        targetdir ("Lib"..the_vs_name.."/%{cfg.buildcfg}")
    filter {'kind:StaticLib',"platforms:x64"}
        targetdir ("Lib"..the_vs_name.."/%{cfg.buildcfg}x64")

    --filter {"platforms:Win32"}
    --    libdirs {"Lib"..the_vs_name.."/%{cfg.buildcfg}"}
    --filter {"platforms:x64"}
    --    libdirs {"Lib"..the_vs_name.."/%{cfg.buildcfg}x64"}

if g_use_pmakeprj then
project '_Pmake'
    kind 'Utility'
    files {'Premake5.lua','Premake5Sln.lua'}
	files {'**.lua'}
    vpaths {["*"] = {'**.lua'}}
    postbuildcommands {
		"cd /d ..\\",
		"Premake5 ".._ACTION
	}
    cleancommands {"cd /d ..\\"}
end 
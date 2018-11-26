


local TheTargetName='lua'

if lua_build_share then
project (TheTargetName)
	language 'C'
    targetname (TheTargetName)
    kind 'SharedLib'
    includedirs {'../'..TheTargetName,"../"}
    files {'../'..TheTargetName..'/**','**'}           
    defines {the_dll_export_define}
end

if lua_build_static then
project (TheTargetName..'Static')
	language 'C'
    targetname (TheTargetName..'Static')
    kind 'StaticLib'
    includedirs {'../'..TheTargetName,"../"}
    files {'../'..TheTargetName..'/**','**'}
end


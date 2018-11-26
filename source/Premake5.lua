

local TheTargetName='MMathPlot'

dofile('lua5.3.4/Premake5.lua')
dofile('test/Premake5.lua')

project (TheTargetName)
    targetname (TheTargetName)
    kind ('WindowedApp')
    linkoptions {'/ENTRY:wWinMainCRTStartup'}

    includedirs {'./'}
	includedirs {'./MainFrm'}

	resincludedirs {'../'}
    
	vpaths { ["res/*"] = "res/**" }
	vpaths { ["*"] = "**" }

    files {'**'}
    removefiles {'**.aps'}
	removefiles {'lua5.3.4/**'}
	removefiles {'test/**'}
	
	pm_lib_use_lua('static')

    defines {'_AFXDLL'}
    
    pchheader ('stdafx.h')
    pchsource ('stdafx.cpp')
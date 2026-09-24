-- include subprojects
includes("lib/commonlibsse")

-- set project constants
set_project("BakaHeroMenu")
set_version("1.0.0")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- define targets
target("BakaHeroMenu")
    add_rules("commonlibsse.plugin", {
        name = "BakaHeroMenu",
        author = "shad0wshayd3",
        xse_minimum = "2.3.0"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

    -- add extra files
    add_extrafiles(".clang-format")

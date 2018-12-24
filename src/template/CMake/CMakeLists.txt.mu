cmake_minimum_required(VERSION 2.4)

#��Ŀ����
project(project_name)

#���⾯��
if(COMMAND cmake_policy)
     cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#���ð汾��
set(project_name_VERSION_MAJOR	1)
set(project_name_VERSION_MINOR	0)
set(project_name_VERSION_PATCH	0)
set(project_name_VERSION_REVISION	0000)

find_package(PkgConfig REQUIRED)

#����ѡ��
option(example "default OFF" OFF)
option(example2 "default ON" ON)

#Ѱ��ʧ����cmake�˳�
pkg_search_module(GLIB2 glib-2.0 REQUIRED)

#ͷ�ļ�����
include_directories(${PROJECT_SOURCE_DIR}
					${PROJECT_BINARY_DIR}

					${GLIB2_INCLUDE_DIRS}
					/xxx/include
)

#����·������
link_directories(/usr/local/lib/mariadb ${GLIB2_LIBRARIES})

#���ñ���ѡ��
add_compile_options(-Wall -Werror -O3)

#�Զ����·������
find_library(LIBXXXLOG_PATH xxxlog /xxx/libs)
if(NOT LIBXXXLOG_PATH)
MESSAGE(FATAL_ERROR "libxxxlog not found")
endif(NOT LIBXXXLOG_PATH)

#�������ӿ�
set(link_libs ${LIBXXXLOG_PATH})

#config.h����
configure_file(
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)

#����Դ�ļ�
set(file_FILES

)


#���ÿ�ִ�г�������
set(target_name xxx)
#���ÿ�����
set(lib_name	yyy)

#���ɿ�ִ���ļ�
add_executable(${target_name} ${file_FILES})
target_link_libraries(${target_name} ${link_libs})

#���ɶ�̬��
add_library(${lib_name} SHARED ${file_FILES})
target_link_libraries(${lib_name} ${link_libs})
set_target_properties(${lib_name} 
			PROPERTIES VERSION 	${project_name_VERSION_MAJOR}.${project_name_VERSION_MINOR}.${project_name_VERSION_PATCH}.${project_name_VERSION_REVISION}}
			SOVERSION 1
)

#�򿪲��ԣ�Ŀǰ��cmake��ctest�˽ⲻ�࣬�ݲ��ܽ�
#enable_testing()

#��װ
#set(CMAKE_INSTALL_PREFIX /iridium)
set(DESTDIR	/iridium)
install(FILES ${PROJECT_SOURCE_DIR}/file.h DESTINATION include/xxxx)		#ͷ�ļ�
install(FILES ${PROJECT_SOURCE_DIR}/config/xxx.common DESTINATION config)	#�����ļ�

install(TARGETS ${target_name} DESTINATION bin)			#��ִ�г���
install(TARGETS ${lib_name} DESTINATION libs)		#��
install(FILES ${PROJECT_BINARY_DIR}/config.h DESTINATION include/config.h.xxx)	#��Ŀ�����ļ��������ڰ�װ�����в鿴�汾

# ����һ�� CPack ��װ��
include (InstallRequiredSystemLibraries)
set(CPACK_PACKAGING_INSTALL_PREFIX /iridium)
set(CPACK_SET_DESTDIR false)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "description")
set(CPACK_PACKAGE_VENDOR "WeTest")
set(CPACK_PACKAGE_VERSION "${project_name_VERSION_MAJOR}.${project_name_VERSION_MINOR}.${project_name_VERSION_PATCH}.${project_name_VERSION_REVISION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${project_name_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${project_name_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${project_name_VERSION_PATCH}")
set(CPACK_RPM_PACKAGE_GROUP "${PROJECT_NAME}")
set(CPACK_RPM_PACKAGE_URL "http://${PROJECT_NAME}.com")
set(CPACK_RPM_PACKAGE_DESCRIPTION "component")
set(CPACK_PACKAGE_RELEASE "${project_name_VERSION_MAJOR}")
set(CPACK_RPM_PACKAGE_LICENSE "${PROJECT_NAME} Licence")

set (CPACK_GENERATOR "RPM")
include (CPack)


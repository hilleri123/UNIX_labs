PROJECT (lib C)



set (files
	functions.h
	pull.c
	push.c
	status.c
	create.c
	delete.c)

add_library (${PROJECT_NAME} ${files})

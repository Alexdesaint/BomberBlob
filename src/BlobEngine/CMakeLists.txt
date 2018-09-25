add_library(BlobEngine STATIC
		Window.cpp
		)

target_link_libraries(BlobEngine sfml-system sfml-graphics sfml-window)
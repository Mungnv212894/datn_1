idf_component_register(SRCS "main.c"
                            "../lib/wifi.c"
                            "../lib/adc.c"
                            "../lib/Timestamp.c"
                            #"../lib/thingsboard"
                    INCLUDE_DIRS "." 
                                    "../lib")
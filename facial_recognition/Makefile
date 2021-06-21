libs = -lpng -lgsl -lgslcblas
all: lock_server trainer
.PHONY: all

# facial_recognition: facial_recognition.c
# 	gcc facial_recognition.c -o facial_recognition $(libs)

lock_server: lock_server.c
	gcc lock_server.c facial_recognition.c  mini_uart.c -o lock_server $(libs)

trainer: trainer.c
	gcc trainer.c facial_recognition.c  -o trainer $(libs)


clean:
	$(RM) *.o lock_server trainer &> /dev/null || true
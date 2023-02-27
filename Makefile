build :
	clang game.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
deploy : build
	rm -rf project-banana
	mkdir project-banana
	cp -r assets project-banana
	cp a.out project-banana/game
	zip -r project-banana.zip project-banana
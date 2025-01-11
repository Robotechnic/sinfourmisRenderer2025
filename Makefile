docker:
	docker run -it --mount type=bind,src=.,dst=/sinfourmis,bind-propagation=rshared sinfourmis_render:v1

build:
	docker build -t sinfourmis_render:v1 .
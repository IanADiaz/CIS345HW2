struct qlock {
	struct spinlock lk;
	int locked;
	char *name;
};



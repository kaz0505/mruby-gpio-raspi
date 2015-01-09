int set_rpin(int pin)
{
	switch(pin){
		case 0:
			return 2;
		case 1:
			return 3;
		case 2:
			return 4;
		case 3:
			return 14;
		case 4:
			return 15;
		case 5:
			return 17;
		case 6:
			return 18;
		case 7:
			return 21;
		case 8:
			return 22;
		case 9:
			return 23;
		case 10:
			return 24;
		case 11:
			return 10;
		case 12:
			return 9;
		case 13:
			return 25;
		case 14:
			return 11;
		case 15:
			return 8;
		case 16:
			return 7;
		default:
			printf("error : pin %d is wrong number", pin);
			return -1;
	}
}

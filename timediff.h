#ifndef TIMEDIFF_H
#define TIMEDIFF_H

	class timediff
	{
		public:
			timediff();
			static double get_now();
			double get_diff();
			double get_start();
		private:
			double _start;
			double _last_checkpoint;
	};

#endif //TIMEDIFF_H

#ifndef CONFFILEREAD_H
#define CONFFILEREAD_H

namespace muduo{
class ConfFileRead{
	public:
		//默认打开INFO级别日志
		ConfFileRead(const char * FilePath, int level=2, int LogFileSize = 524288000);
		
		int GetLevel() { return level_; }
		int GetFileSize() { return LogFileSize_; }
	
	private:
		int level_;
		int LogFileSize_;
};

}

#endif /* CONFFILEREAD_H */
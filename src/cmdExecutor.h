#pragma once

#include <memory>
#include <string>

class CmdExecutor {
    public:
        virtual const std::string runCmd(const char* cmd) = 0;
        virtual ~CmdExecutor() = default;
};

class UnixCmdExecutor : public CmdExecutor {
    public:
        const std::string runCmd(const char* cmd) override;
};

class DosCmdExecutor : public CmdExecutor {
    public:
        const std::string runCmd(const char* cmd) override;
};

class CmdExecutorFactory {
    public:
        virtual std::unique_ptr<CmdExecutor> Create() = 0;
        virtual ~CmdExecutorFactory() = default;
};

class CmdExecutorCreator : public CmdExecutorFactory {
    public:
        std::unique_ptr<CmdExecutor> Create() override;
};

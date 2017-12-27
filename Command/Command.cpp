#include <iostream>
#include <memory>
#include <stack>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//Example of Command Design Pattern
//actors:
//
//ImageProcessor - receiver of the command,
//                 there can be a lot of them (i.e. multithreaded image processor, logging image processor)
//
//Command Stack  - invoker of the command, stores the commands, it can be GUI, or ThreadPool etc.
//
//main()         - client, creates concrete commands, sets receivers

class ImageProcessor
{
public:
    Mat resize(Mat src, double factor)
    {
        Mat dst;
        cv::resize(src, dst, Size(0, 0), factor, factor);
        return dst;
    }

    Mat translate(Mat src, int x, int y)
    {
        Mat dst;

        Mat tr = Mat::eye( 2, 3, CV_32FC1 );
        // affine_matrix[0][2]:
        *(((float*) (tr.data))+2) = x;
        // affine_matrix [1][2]:
        *(((float*) (tr.data))+5) = y;

        warpAffine(src, dst, tr, Size(src.cols, src.rows));
        return dst;
    }

    Mat rotate(Mat src, double angle)
    {
        Mat dst;
        Point2f pt(src.cols/2., src.rows/2.);
        Mat r = getRotationMatrix2D(pt, angle, 1.0);
        warpAffine(src, dst, r, Size(src.cols, src.rows));
        return dst;
    }
};

class Command
{
public:
    Command(ImageProcessor& proc, Mat& theDst, std::string theDesc) : receiver(proc), dst(theDst), desc(theDesc) {}
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    std::string str()
    {
        return desc;
    }

protected:
    ImageProcessor& receiver;
    Mat& dst;
    std::string desc = "Undefined";
};

class MoveRight : public Command
{
public:
    MoveRight(ImageProcessor& proc,  Mat& theDst)
    : Command(proc, theDst, "MoveRight") {}

    void execute() override
    {
        dst = receiver.translate(dst, 20, 0);
    }

    void undo() override
    {
        dst = receiver.translate(dst, -20, 0);
    }
};

class RotateTenDegree : public Command
{
public:
    RotateTenDegree(ImageProcessor& proc, Mat& theDst)
    : Command(proc, theDst, "Rotate 10 degr.") {}

    void execute() override
    {
        snapshots.push(dst);
        dst = receiver.rotate(dst, 10);
    }

    void undo() override
    {
        if(!snapshots.empty())
        {
            dst = snapshots.top();
            snapshots.pop();
        }
    }
private:
    std::stack<Mat> snapshots;
};

class MacroCommand : public Command
{
public:
    MacroCommand(ImageProcessor& proc, Mat& theDst, std::vector<Command*> theCommands) : Command(proc, theDst, "Ultra powerful macro"),
                                                                                   commands(theCommands) {}

    void execute() override
    {
        for(auto cmd : commands)
        {
            cmd->execute();
        }
    }

    void undo() override
    {
        for(auto cmd = commands.rbegin(); cmd != commands.rend(); ++cmd)
        {
            (*cmd)->undo();
        }
    }
private:
    std::vector<Command*> commands;
};

class CommandStack
{
public:
    void execute(Command* cmd)
    {
        cmd->execute();
        commands.push(cmd);
        descriptions.push_back(cmd->str());
    }

    void undo()
    {
        if(!commands.empty() && !descriptions.empty())
        {
            commands.top()->undo();
            commands.pop();
            descriptions.pop_back();
        }
    }
    void printCommands()
    {
        std::cout << "Command stack:\n";
        for(auto cmd : descriptions)
        {
            std::cout << "\t" << cmd << "\n";
        }
    }
private:
    std::stack<Command*> commands;
    std::vector<std::string> descriptions;
};

void cls()
{
    std::cout << "\033[2J\033[1;1H";
}

void printControls()
{
    cls();
    std::cout << "Controls: \n";
    std::cout << "m - move image 20 pixels right\n";
    std::cout << "r - rotate image 10 degrees counter clockwise\n";
    std::cout << "3 - powerful macro\n";
    std::cout << "u - undo last operation\n";
    std::cout << "q - exit\n";
}

int main(int argc, char* argv[])
{
    Mat src = imread("logo.jpg");
    int padding = 200;
    Mat canvas(src.rows + padding, src.cols + padding, src.type());
    src.copyTo(canvas(cv::Rect(padding/2, padding/2, src.cols, src.rows)));

    ImageProcessor receiver;
    CommandStack invoker;
    Mat result(canvas);

    MoveRight moveCmd(receiver, result);
    RotateTenDegree rotateCmd(receiver, result);
    MacroCommand macroCmd(receiver, result, {&moveCmd, &rotateCmd, &rotateCmd});

    imshow("Result", result);
    printControls();
    waitKey(100);

    char ans = 0;
    while(std::cin >> ans)
    {
        switch(ans)
        {
            case 'm':
                invoker.execute(&moveCmd);
                break;

            case 'r':
                invoker.execute(&rotateCmd);
                break;

            case '3':
                invoker.execute(&macroCmd);
                break;

            case 'u':
                invoker.undo();
                break;

            case 'q':
                cls();
                return 0;
            default:
                break;
        }
        imshow("Result", result);
        printControls();
        invoker.printCommands();
        waitKey(100);
    }

}

#include "ShapesController.h"

ShapesController::ShapesController(std::istream& input, std::ostream& output)
	: m_inputStream(input)
	, m_outputStream(output)
	, m_shapes({})
	, m_actionMap({ { "line", bind(&ShapesController::CreateLine, this, std::placeholders::_1) },
		  { "circle", bind(&ShapesController::CreateCircle, this, std::placeholders::_1) },
		  { "rectangle", bind(&ShapesController::CreateRectangle, this, std::placeholders::_1) },
		  { "triangle", bind(&ShapesController::CreateTriangle, this, std::placeholders::_1) },
		  { "help", bind(&ShapesController::Help, this, std::placeholders::_1) },
		  { "printAllShapes", bind(&ShapesController::PrintAllShapes, this, std::placeholders::_1) } })
{
}

bool ShapesController::HandleCommand()
{
	std::string commandLine;
	std::getline(m_inputStream, commandLine);
	std::istringstream strm(commandLine);

	std::string action;
	strm >> action;

	auto it = m_actionMap.find(action);

	if (it != m_actionMap.end())
	{
		return it->second(strm);
	}

	return false;
}

bool ShapesController::CreateLine(std::istream& args)
{
	if (args.eof())
	{
		m_outputStream << "Correct usage:" << std::endl
					   << "   line <startPoint X> <startPoint Y> <endPoint X> <endPoint Y> <hex outlineColor?>" << std::endl;
		return false;
	}

	ArgsLine argsLine;
	args >> argsLine.startPoint >> argsLine.endPoint >> std::hex >> argsLine.outlineColor;

	if (args.fail())
	{
		m_outputStream << "Correct usage:" << std::endl
					   << "   - line <startPoint X> <startPoint Y> <endPoint X> <endPoint Y> <hex outlineColor?>" << std::endl
					   << "   - startPoint's & endPoint's coordinates must be a number" << std::endl
					   << "   - outlineColor must be >= 0 and <= ffffff " << std::endl;
		return false;
	}

	CLineSegment line(argsLine.startPoint, argsLine.endPoint, argsLine.outlineColor);
	m_shapes.push_back(std::make_shared<CLineSegment>(line));
	m_outputStream << "Line has been created." << std::endl;
	return true;
}

bool ShapesController::CreateCircle(std::istream& args)
{
	if (args.eof())
	{
		m_outputStream << "Correct usage:" << std::endl
					   << "   circle <centerPoint X> <centerPoint Y> <radius> <hex fillColor?> <hex outlineColor?>" << std::endl;
		return false;
	}

	ArgsCircle argsCircle;
	args >> argsCircle.centerPoint >> argsCircle.radius >> std::hex >> argsCircle.fillColor >> argsCircle.outlineColor;

	if (args.fail())
	{
		m_outputStream
			<< "Correct usage:" << std::endl
			<< "   - circle <centerPoint X> <centerPoint Y> <radius> <hex fillColor?> <hex outlineColor?>" << std::endl
			<< "   - centerPoint's coordinates and radius must be a number" << std::endl
			<< "   - fillColor & outlineColor must be >= 0 and <= ffffff " << std::endl;
		return false;
	}

	CCircle circle(argsCircle.centerPoint, argsCircle.radius, argsCircle.fillColor, argsCircle.outlineColor);
	m_shapes.push_back(std::make_shared<CCircle>(circle));
	m_outputStream << "Circle has been created." << std::endl;
	return true;
}

bool ShapesController::CreateRectangle(std::istream& args)
{
	if (args.eof())
	{
		m_outputStream << "Correct usage:" << std::endl
					   << "   rectangle <leftTopPoint X> <leftTopPointY> <rightBottomPoint X> <rightBottomPoint Y> <hex fillColor?> <hex outlineColor?>" << std::endl;
		return false;
	}

	ArgsRectangle argsRectangle;
	args >> argsRectangle.leftTopPoint >> argsRectangle.rightBottomPoint >> std::hex >> argsRectangle.fillColor >> argsRectangle.outlineColor;

	if (args.fail())
	{
		m_outputStream
			<< "Correct usage:" << std::endl
			<< "   - rectangle <leftTopPoint X> <leftTopPointY> <rightBottomPoint X> <rightBottomPoint Y> <hex fillColor?> <hex outlineColor?>" << std::endl
			<< "   - leftTop & rightBottom coordinates must be a number" << std::endl
			<< "   - fillColor & outlineColor must be >= 0 and <= ffffff " << std::endl;
		return false;
	}

	CRectangle rectangle(argsRectangle.leftTopPoint, argsRectangle.rightBottomPoint, argsRectangle.fillColor, argsRectangle.outlineColor);

	m_shapes.push_back(std::make_shared<CRectangle>(rectangle));
	m_outputStream << "Rectangle has been created." << std::endl;
	return true;
}

bool ShapesController::CreateTriangle(std::istream& args)
{
	if (args.eof())
	{
		m_outputStream << "Correct usage:" << std::endl
					   << "   triangle <vertex1 X> <vertex1 Y> <vertex2 X> <vertex2 Y> <vertex3 X> <vertex3 Y> <hex fillColor?> <hex outlineColor?>" << std::endl;
		return false;
	}

	ArgsTriangle argsTriangle;
	args >> argsTriangle.vertex1 >> argsTriangle.vertex2 >> argsTriangle.vertex3 >> std::hex >> argsTriangle.fillColor >> argsTriangle.outlineColor;

	if (args.fail())
	{
		m_outputStream << "Correct usage:" << std::endl
					   << "   - triangle <vertex1 X> <vertex1 Y> <vertex2 X> <vertex2 Y> <vertex3 X> <vertex3 Y> <hex fillColor?> <hex outlineColor?>" << std::endl
					   << "   - vertexes's coordinates must be a number" << std::endl
					   << "   - fillColor & outlineColor must be >= 0 and <= ffffff " << std::endl;
		return false;
	}

	CTriangle triangle(argsTriangle.vertex1, argsTriangle.vertex2, argsTriangle.vertex3, argsTriangle.fillColor, argsTriangle.outlineColor);
	m_shapes.push_back(std::make_shared<CTriangle>(triangle));
	m_outputStream << "Rectangle has been created." << std::endl;

	return true;
}

void ShapesController::PrintMaxAreaShape()
{
	if (m_shapes.size() == 0)
	{
		m_outputStream << "There are no shapes yet" << std::endl;
		return;
	}

	auto maxAreaShapeIt = std::max_element(m_shapes.begin(),
		m_shapes.end(),
		[](const std::shared_ptr<IShape>& shape1, const std::shared_ptr<IShape>& shape2) { return shape1->GetArea() < shape2->GetArea(); });
	m_outputStream << "Max Area Shape:" << std::endl
				   << (*maxAreaShapeIt)->ToString() << std::endl;
}

void ShapesController::PrintMinPerimeterShape()
{
	if (m_shapes.size() == 0)
	{
		m_outputStream << "There are no shapes yet" << std::endl;
		return;
	}

	auto minPerimeterShape = std::max_element(m_shapes.begin(),
		m_shapes.end(),
		[](const std::shared_ptr<IShape>& shape1, const std::shared_ptr<IShape>& shape2) { return shape1->GetPerimeter() > shape2->GetPerimeter(); });
	m_outputStream << "Min Perimeter Shape:" << std::endl
				   << (*minPerimeterShape)->ToString() << std::endl;
}

bool ShapesController::PrintAllShapes(std::istream& args)
{
	for (auto& shape : m_shapes)
	{
		m_outputStream << shape->ToString() << std::endl;
	}
	return true;
}

bool ShapesController::Help(std::istream& args)
{
	std::ostringstream helpStream;
	m_outputStream << std::fixed << std::setprecision(2)
				   << "AVAILABLE COMMANDS:" << std::endl
				   << "-> info: show available commands" << std::endl
				   << "-> rectangle <leftTopPoint X> <leftTopPointY> <rightBottomPoint X>" << std::endl
				   << "<rightBottomPoint Y> <hex fillColor?> <hex outlineColor?>" << std::endl
				   << "   - creating rectangle using two points's coordinates " << std::endl
				   << "     and optional color set" << std::endl
				   << std::endl
				   << "-> circle <centerPoint X> <centerPoint Y> " << std::endl
				   << "<radius> <hex fillColor?> <hex outlineColor?>" << std::endl
				   << "   - creating circle using centerPoint's coordinates & radius, " << std::endl
				   << "     and optional color set" << std::endl
				   << std::endl
				   << "-> triangle <vertex1 X> <vertex1 Y> <vertex2 X> " << std::endl
				   << "<vertex2 Y> <vertex3 X> <vertex3 Y> <hex fillColor?> <hex outlineColor?>" << std::endl
				   << "   - creating triangle with three vertexes's coordinates " << std::endl
				   << "     and optional color set" << std::endl
				   << std::endl
				   << "-> line <startPoint X> <startPoint Y> <endPoint X> " << std::endl
				   << "<endPoint Y> <hex outlineColor?>" << std::endl
				   << "   - creating line with two point's coordinates " << std::endl
				   << "     and optional color set" << std::endl
				   << std::endl
				   << "-> maxarea" << std::endl
				   << "   - finding max area of all available shapes " << std::endl
				   << "-> minperimeter" << std::endl
				   << "   - finding min perimeter of all available shapes " << std::endl;
	return true;
}

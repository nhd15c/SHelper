#include <QDebug>
#include <QtCharts/qchart.h>
#include <QtCharts/qlineseries.h>
#include <QLineF>
#include "include/lpgraphicalm.h"
#include "ui_lpgraphicalm.h"

using namespace QtCharts;

// TODO: Check the math.
LPGraphicalM::LPGraphicalM(std::vector<std::vector<double>> coefficientGroup, QWidget *parent) :
	m_coefficientGroup(coefficientGroup), QDialog(parent),
	ui(new Ui::LPGraphicalM)
{
	ui->setupUi(this);

	for(unsigned int crn = 1; crn < m_coefficientGroup.size(); crn++)
	{
		std::vector<double> restriction = m_coefficientGroup.at(crn);
		determinePoints(restriction);
	}
	getVertex();
	graphicate();
}

LPGraphicalM::~LPGraphicalM()
{
	delete ui;
}

/* m_restrictionPoints hold all the QPointF to create the constraint lines.
 * Each line uses two elements of the vector.
 */
void LPGraphicalM::determinePoints(std::vector<double> restriction)
{
	double X = restriction.at(0);
	double Y = restriction.at(1);
	double result = restriction.at(2);

	QPointF pointY(0, result/Y);
	m_restrictionPoints.push_back(pointY);
	QPointF pointX(result/X, 0);
	m_restrictionPoints.push_back(pointX);
	qDebug() << "PointY:" << pointY.rx() << " : " <<
				pointY.ry() << "PointX:" << pointX.rx() << " : " << pointX.ry();
}

void LPGraphicalM::graphicate()
{
	QChart *chart = new QChart();
	std::vector<QPointF>::const_iterator pointItr;
	int restrictionNumber(1);
	for(pointItr = m_restrictionPoints.begin(); pointItr != m_restrictionPoints.end(); pointItr++)
	{
		QLineSeries *constraint = new QLineSeries();
		constraint->append(*pointItr);
		constraint->append(*(++pointItr));
		constraint->setName("R" + restrictionNumber);
		chart->addSeries(constraint);
	}
	chart->createDefaultAxes();
	ui->chartView->setChart(chart);
}

void LPGraphicalM::getVertex()
{
	// TODO: Finish this algorithm. It's not generating the vertices for 0Y or 0X.
	// NOTE: I haven't checked if the vertex generated are correct.
	// FIXME: Repeated elements in the vertices "vector".
	std::vector<QPointF>::const_iterator pointItr(m_restrictionPoints.begin());
	std::vector<QLineF> constraintLines;
	for(; pointItr != m_restrictionPoints.end(); pointItr+=2)
	{
		QLineF tempLine(*pointItr, *(pointItr+1));
		constraintLines.push_back(tempLine);
	}

	std::vector<QLineF>::const_iterator lineItr(constraintLines.begin());
	for(; lineItr != constraintLines.end(); lineItr++)
	{
		QPointF tempPoint;
		QLineF tempLine(*lineItr); // Gets a constraint QLineF.
		std::vector<QLineF>::const_iterator nextLineItr(constraintLines.begin());

		// NOTE: Probably not very optimal.
		for(; nextLineItr != constraintLines.end(); nextLineItr++) // Check lineItr against every other one.
		{
			//if(nextLineItr != lineItr) // Don't compare a line with itself.
			tempLine.intersect(*nextLineItr, &tempPoint);
			if(!tempPoint.isNull())
				m_vertexPoints.push_back(tempPoint);
		}
	}

	qDebug() << "\n";
	std::vector<QPointF>::const_iterator debugItr(m_vertexPoints.begin());
	for(; debugItr != m_vertexPoints.end(); debugItr++)
		qDebug() << *debugItr;
}

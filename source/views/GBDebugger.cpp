// GBDebugger.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBRobot.h"
#include "GBWorld.h"
#include "GBDebugger.h"
#include "GBStackBrain.h"


const short kEdgeSpace = 2;
const short kStatusBoxHeight = 30;
const short kPCBoxHeight = 75;
const short kStackBoxHeight = 65;
const short kPrintBoxHeight = 15;

void GBDebuggerView::DrawStatusBox(const GBRect & box) {
	DrawBox(box);
	DrawStringLeft(target->Name(), box.left + 4, box.top + 13, 12);
	const GBBrain * brain = target->Brain();
	const GBStackBrain * sbrain = dynamic_cast<const GBStackBrain *>(brain);
	DrawStringLeft(brain ? (sbrain ? "Stack brain." : "Unknown brain.") : "No brain.",
		box.left + 3, box.bottom - 4, 10);
	if ( brain )
		DrawStringPair("Status:", brain->StatusName(),
			(box.left + box.right + 3) / 2, box.right - 3, box.bottom - 4,
			10, brain->Status() == bsOK ? GBColor::darkGreen :
				brain->Status() == bsError ? GBColor::darkRed : GBColor::black);
}

void GBDebuggerView::DrawPCBox(const GBRect & box, const GBStackBrain * brain) {
	DrawBox(box);
	GBStackAddress pc = brain->PC();
	DrawStringPair("PC:", ToString(pc) + " (line " + ToString(brain->PCLine()) + ')',
		box.left + 3, box.right - 3, box.top + 11, 10, GBColor::black, true);
	for ( long i = -3; i <= 2; i ++ )
		if ( brain->ValidAddress(pc + i) )
			DrawStringPair(brain->AddressName(pc + i) + ':', brain->DisassembleAddress(pc + i),
				box.left + 3, box.right - 3, box.bottom - 24 + 10 * i, 10,
				i == 0 ? GBColor::blue : GBColor::black);
}

void GBDebuggerView::DrawStackBox(const GBRect & box, const GBStackBrain * brain) {
	DrawBox(box);
	DrawStringLeft("Stack:", box.left + 3, box.top + 11, 10, GBColor::black, true);
	long height = brain->StackHeight();
	if ( height ) {
		for ( long i = 0; i < 5 && i < height; i ++ )
			DrawStringPair(ToString(height - i) + ':', ToString(brain->StackAt(height - i - 1)),
				box.left + 3, box.right - 3, box.bottom - 44 + 10 * i, 10);
	} else
		DrawStringRight("empty", box.right - 3, box.top + 31, 10);
}

void GBDebuggerView::DrawReturnStackBox(const GBRect & box, const GBStackBrain * brain) {
	DrawBox(box);
	DrawStringLeft("Return stack:", box.left + 3, box.top + 11, 10, GBColor::black, true);
	long height = brain->ReturnStackHeight();
	if ( height ) {
		for ( long i = 0; i < 5 && i < height; i ++ )
			DrawStringPair(ToString(height - i) + ':', ToString(brain->ReturnStackAt(height - i - 1)),
				box.left + 3, box.right - 3, box.bottom - 44 + 10 * i, 10);
	} else
		DrawStringRight("empty", box.right - 3, box.top + 31, 10);
}

void GBDebuggerView::DrawVariablesBox(const GBRect & box, const GBStackBrain * brain) {
	GBSymbolIndex vars = brain->NumVariables();
	GBSymbolIndex vvars = brain->NumVectorVariables();
	if ( vars || vvars ) {
		DrawBox(box);
		short y = box.top + 11;
		DrawStringLeft("Variables:", box.left + 3, y, 10, GBColor::black, true);
		for ( GBSymbolIndex i = 0; i < vars; i ++ ) {
			y += 10;
			DrawStringPair(brain->VariableName(i), ToString(brain->ReadVariable(i)),
				box.left + 3, box.right - 3, y, 10);
		}
		for ( GBSymbolIndex i = 0; i < vvars; i ++ ) {
			y += 10;
			DrawStringPair(brain->VectorVariableName(i), ToString(brain->ReadVectorVariable(i)),
				box.left + 3, box.right - 3, y, 10);
		}
	} else
		DrawStringLeft("No variables", box.left + 3, box.top + 11, 10);
}

void GBDebuggerView::DrawPrintBox(const GBRect & box, const GBStackBrain * brain) {
	const std::string & print = brain->LastPrint();
	if ( print != "none" )
		DrawBox(box);
	DrawStringPair("Last print:", print, box.left + 3, box.right - 3, box.top + 11, 10);
}

void GBDebuggerView::UpdateTarget() {
	if ( (GBObject *)target == world.Followed() ) return;
	if ( target ) target->RemoveDeletionListener(this);
	target = dynamic_cast<GBRobot *>(world.Followed());
	if ( target ) target->AddDeletionListener(this);
}

GBDebuggerView::GBDebuggerView(GBWorld & wld)
	: target(nil),
	world(wld), worldChanges(-1), redrawAnyway(true)
{}

GBDebuggerView::~GBDebuggerView() {}

bool GBDebuggerView::Active() const {
	const_cast<GBDebuggerView *>(this)->UpdateTarget();
	return target && target->Brain();
}

void GBDebuggerView::StartStopBrain() {
	UpdateTarget();
	if ( ! target ) return;
	GBBrain * brain = target->Brain();
	if ( ! brain ) return;
	brain->SetStatus(brain->Status() == bsOK ? bsStopped : bsOK);
}

bool GBDebuggerView::Step() {
	UpdateTarget();
	if ( ! target ) return true;
	GBBrain * brain = target->Brain();
	if ( ! brain ) return true;
	if ( ! brain->Ready() ) return true;
	redrawAnyway = true;
	brain->Step(target, &world);
	return ! brain->Ready();
}

void GBDebuggerView::ReportDeletion(const GBDeletionReporter * deletee) {
	if ( deletee == (const GBDeletionReporter *)target ) {
		target = nil;
		redrawAnyway = true;
	} else
		throw GBGenericError("Deletion reported on object GBDebuggerView isn't viewing.");
}

GBMilliseconds GBDebuggerView::RedrawInterval() const {
	return 200;
}

bool GBDebuggerView::InstantChanges() const {
	return redrawAnyway || (GBObject *)target != world.Followed();
}

bool GBDebuggerView::DelayedChanges() const {
	return target && world.ChangeCount() != worldChanges;
}

void GBDebuggerView::Draw() {
	UpdateTarget();
	DrawBackground();
	if ( ! target ) {
		DrawStringLeft("Nothing selected", 4, 20, 12);
	} else {
		GBRect box;
	// draw robot name
		box.left = box.top = kEdgeSpace;
		box.right = Width() - kEdgeSpace;
		box.bottom = box.top + kStatusBoxHeight;
		DrawStatusBox(box);
	// get brain
		const GBStackBrain * sbrain = dynamic_cast<GBStackBrain *>(target->Brain());
		if ( sbrain ) {
		// draw pc
			box.top = box.bottom + kEdgeSpace;
			box.bottom = box.top + kPCBoxHeight;
			DrawPCBox(box, sbrain);
		// draw stack
			box.top = box.bottom + kEdgeSpace;
			box.bottom = box.top + kStackBoxHeight;
			box.right = (Width() - kEdgeSpace) / 2;
			DrawStackBox(box, sbrain);
		// draw return stack
			box.left = (Width() + kEdgeSpace) / 2;
			box.right = Width() - kEdgeSpace;
			DrawReturnStackBox(box, sbrain);
		// draw variables
			box.top = box.bottom + kEdgeSpace;
			box.bottom = box.top + (sbrain->NumVariables() + sbrain->NumVectorVariables()) * 10 + 15;
			box.left = kEdgeSpace;
			DrawVariablesBox(box, sbrain);
		// draw prints
			box.top = box.bottom + kEdgeSpace;
			box.bottom = box.top + kPrintBoxHeight;
			DrawPrintBox(box, sbrain);
		}
	}
// record
	worldChanges = world.ChangeCount();
	redrawAnyway = false;
}

short GBDebuggerView::PreferredWidth() const {
	return 250;
}

short GBDebuggerView::PreferredHeight() const {
	if ( target ) {
		const GBStackBrain * sbrain = dynamic_cast<GBStackBrain *>(target->Brain());
		if ( sbrain )
			return kStatusBoxHeight + kPCBoxHeight + kStackBoxHeight + kPrintBoxHeight
				+ (sbrain->NumVariables() + sbrain->NumVectorVariables()) * 10 + 15
				+ 6 * kEdgeSpace;
		else
			return kStatusBoxHeight + 2 * kEdgeSpace;
	} else
		return 40;
}

const string GBDebuggerView::Name() const {
	return "Debugger";
}

void GBDebuggerView::AcceptClick(short x, short y, int /*clicks*/) {
	UpdateTarget();
	if ( target && x > Width() / 2 && x < Width() - kEdgeSpace
			&& y > kEdgeSpace + kStatusBoxHeight - 10 && y < kEdgeSpace + kStatusBoxHeight )
		StartStopBrain();
}

void GBDebuggerView::AcceptKeystroke(const char what) {
	if ( what == 's' || what == 'S' )
		StartStopBrain();
}
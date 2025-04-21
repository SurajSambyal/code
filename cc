import simplegui

# Grading interface equivalent
def calculate_grade(percentage):
    if percentage >= 85:
        return "A"
    elif percentage >= 70:
        return "B"
    elif percentage >= 50:
        return "C"
    else:
        return "F"

class Person:
    def __init__(self, name="Unknown", age=0):
        self.name = name
        self.age = age

class StudentBase(Person):
    student_count = 0
    
    def __init__(self, name, age, student_id):
        super().__init__(name, age)
        self.student_id = student_id
        StudentBase.student_count += 1
    
    @classmethod
    def show_student_count(cls):
        return f"Total Students: {cls.student_count}"

class GraduateStudent(StudentBase):
    def __init__(self, name, age, student_id, percentage):
        super().__init__(name, age, student_id)
        self.percentage = percentage
        self.grade = calculate_grade(percentage)
    
    def get_table_row(self):
        return [self.student_id, self.name, str(self.age), 
                f"{self.percentage:.2f}%", self.grade]

class StudentInfoGUI:
    def __init__(self):
        self.student_list = []
        self.frame = simplegui.create_frame("Student Information System", 800, 600)
        
        # Input fields
        self.name_input = self.frame.add_input("Name:", self.name_handler, 200)
        self.age_input = self.frame.add_input("Age:", self.age_handler, 200)
        self.id_input = self.frame.add_input("Student ID:", self.id_handler, 200)
        self.percentage_input = self.frame.add_input("Percentage:", self.percentage_handler, 200)
        
        # Buttons
        self.frame.add_button("Add Student", self.add_student)
        self.frame.add_button("Delete Selected", self.delete_student)
        
        # Output area
        self.output_label = self.frame.add_label("")
        self.output_text = ""
        
        # Table headers
        self.headers = ["Student ID", "Name", "Age", "Percentage", "Grade"]
        self.selected_index = -1
        
        self.frame.set_mouseclick_handler(self.mouse_handler)
        self.frame.set_draw_handler(self.draw_handler)
        self.frame.start()
    
    def name_handler(self, text):
        pass  # Input handled in add_student
    
    def age_handler(self, text):
        pass  # Input handled in add_student
    
    def id_handler(self, text):
        pass  # Input handled in add_student
    
    def percentage_handler(self, text):
        pass  # Input handled in add_student
    
    def add_student(self):
        try:
            name = self.name_input.get_text().strip()
            age = int(self.age_input.get_text().strip())
            student_id = self.id_input.get_text().strip()
            percentage = float(self.percentage_input.get_text().strip())
            
            if not name or not student_id:
                raise ValueError("Name and Student ID cannot be empty")
            if age < 0:
                raise ValueError("Age cannot be negative")
            if percentage < 0 or percentage > 100:
                raise ValueError("Percentage must be between 0 and 100")
            
            student = GraduateStudent(name, age, student_id, percentage)
            self.student_list.append(student)
            
            # Clear inputs
            self.name_input.set_text("")
            self.age_input.set_text("")
            self.id_input.set_text("")
            self.percentage_input.set_text("")
            
            self.update_output()
            
        except ValueError as e:
            self.output_text = f"Error: {str(e)}"
    
    def delete_student(self):
        if 0 <= self.selected_index < len(self.student_list):
            del self.student_list[self.selected_index]
            StudentBase.student_count -= 1
            self.selected_index = -1
            self.update_output()
    
    def mouse_handler(self, pos):
        # Simple selection logic (for demo purposes)
        row_height = 30
        header_height = 40
        table_top = 200
        
        if table_top <= pos[1] <= table_top + len(self.student_list) * row_height:
            self.selected_index = (pos[1] - table_top) // row_height
            self.update_output()
    
    def update_output(self):
        self.output_text = "Student Information System\n\n"
        self.output_text += StudentBase.show_student_count() + "\n\n"
        
        # Draw table headers
        header_row = " | ".join(f"{h:^15}" for h in self.headers)
        self.output_text += header_row + "\n"
        self.output_text += "-" * len(header_row) + "\n"
        
        # Draw table rows
        for i, student in enumerate(self.student_list):
            row_data = student.get_table_row()
            row_str = " | ".join(f"{d:^15}" for d in row_data)
            if i == self.selected_index:
                row_str = "> " + row_str + " <"  # Mark selected row
            self.output_text += row_str + "\n"
    
    def draw_handler(self, canvas):
        # Draw the output text
        lines = self.output_text.split('\n')
        for i, line in enumerate(lines):
            canvas.draw_text(line, (50, 50 + i * 20), 16, "White")

# Create and run the application
app = StudentInfoGUI()
